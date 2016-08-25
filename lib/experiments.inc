<?php

/**
 * Experiment design, with a simple checkin-checkout mechanism.
 * Whenever a new user arrives, The most 'needed' experiment (i.e. the one with 
 * the least number of responses) will be allocated for that user, and locked for
 * a certain time period allowing her to complete the survey.
 * In the current implementation, the number of concurrent users (within that 
 * time period) is thus bounded by the number of experiments.
 * TODO: specify 'exp' structure.
 * 
 * @author Michael Rubinstein, MIT 2009
 */

include_once('lib/conf.inc');
include_once('lib/utils.inc');

//define('EXP_FILE', 'bin/experiments.dat');
//define('EXP_STATUS_FILE', 'bin/experiments_status.dat');
//define('EXP_NOSRC_STATUS_FILE', 'bin/experiments_nosrc_status.dat');
define('EXP_FILE', 'bin/experiments_4img.dat');
define('EXP_STATUS_FILE', 'bin/experiments_4img_status.dat');
define('EXP_NOSRC_STATUS_FILE', 'bin/experiments_4img_nosrc_status.dat');


Class Experiments {
	
	private static $exp_type2status_file = array(EXP_TYPE_NORMAL => EXP_STATUS_FILE, EXP_TYPE_NOSRC => EXP_NOSRC_STATUS_FILE); 
	
	public static function updateExperiments() {
		$exp = self::loadExperimentsFile('data/experiments');
		Utils::serializeToFile($exp,EXP_FILE);
		
		// create experiments status (will run-over the current one, if exists)
		$expStatus = Array();
		foreach ($exp as $expid => $value) {
			$expStatus[$expid]['count'] = 0;
			$expStatus[$expid]['time'] = 0;
		}
		Utils::serializeToFile($expStatus,EXP_STATUS_FILE);
		Utils::serializeToFile($expStatus,EXP_NOSRC_STATUS_FILE); // another one for the 'blind' version
	}
	
	public static function loadExperiments() {
//		return Utils::unserializeFromFile('bin/experiments.dat');
		return Utils::unserializeFromFile('bin/experiments_4img.dat');
	}
	
	public static function checkout($expid=INVALID, $type=EXP_TYPE_NORMAL) {
		$fp = fopen("bin/lock", "w");
		if (flock($fp, LOCK_EX)) { // do an exclusive lock. TODO: we can use different lock for each type...
			$status_file = self::$exp_type2status_file[$type];
			$expStatus = self::loadExperimentsStatus($status_file);
			self::refreshExperimentsStatus($expStatus, $status_file);
			if ($expid == INVALID) { // find optimal
				$min = PHP_INT_MAX;
				$max = 0;
				foreach ($expStatus as $id => $status) {
					if ($status['time'] == 0 &&
					    $status['count'] < $min) {
						$min = $status['count'];
						$expid = $id;
					}
				}
				if($min < 3){
					foreach ($expStatus as $id => $status) {
						if ($status['time'] == 0 &&
							$status['count'] < 3) {
							$expid = $id;
							break;
						}
					}
				}
			}
			$expStatus[$expid]['time'] = time();
			self::saveExperimentsStatus($expStatus, $status_file);
			flock($fp, LOCK_UN); // release the lock
		}
		fclose($fp);
		
		return $expid;
	}
	
	public static function checkin($expid, $type) {
		$fp = fopen("bin/lock", "w");
		if (flock($fp, LOCK_EX)) { // do an exclusive lock
			$status_file = self::$exp_type2status_file[$type];	
			$expStatus = self::loadExperimentsStatus($status_file);
			$expStatus[$expid]['count']++;
			$expStatus[$expid]['time'] = 0;
			self::saveExperimentsStatus($expStatus, $status_file);
			flock($fp, LOCK_UN); // release the lock
		}
		fclose($fp);	
	}
	
	private static function loadExperimentsFile($filename) {
		$a = Utils::dlmread($filename,' ');

		// parse
		$exp = Array();
		for ($i = 0; $i < sizeof($a); $i++) {
			for ($j = 0; $j < sizeof($a[$i]); $j++) {
				list($did,$desid,$expid) = sscanf($a[$i][$j], "%d(%d,%d)");
				$exp[$i+1][$j] = array($did,$desid,$expid); // NOTE: $i+1 to keep the experiments 1-based 
			}
		}
		
		return $exp;
	}
	
	private static function saveExperimentsStatus($expStatus,$filename) {
		Utils::serializeToFile($expStatus,$filename);
	}
	
	public static function loadExperimentsStatus($filename) {
		return Utils::unserializeFromFile($filename);	
	}

	private static function refreshExperimentsStatus(&$expStatus) {
		$now = time(); // get current time
		foreach ($expStatus as $id => $status) {
			if ($status['time'] > 0 && 
				$now - $status['time'] > 2 * 60 * 60) { // TODO: hardcoded 2 hours
				$expStatus[$id]['time'] = 0;
			}
		}	
	}
}


?>