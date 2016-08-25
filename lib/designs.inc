<?php

/**
 * Experiment design specifications.
 * TODO: specify 'design' structure. 
 * 
 * @author Michael Rubinstein, MIT 2009
 */

include_once('lib/conf.inc');
include_once('lib/utils.inc');

Class Designs {
	
	public static function updateDesigns() {
		$design = Array();
		// TODO: generalize
		$designs[5] = self::loadDesignFile('data/design_5');
		$designs[6] = self::loadDesignFile('data/design_6');
		$designs[7] = self::loadDesignFile('data/design_7');
		$designs[8] = self::loadDesignFile('data/design_8');

		Utils::serializeToFile($designs,'bin/designs.dat');
	}
	
	public static function loadDesigns() {
		return Utils::unserializeFromFile('bin/designs.dat');
	}
	
	private static function loadDesignFile($filename) {
		$a = Utils::dlmread($filename,' ');

		// parse
		$design = Array();
		for ($i = 0; $i < sizeof($a); $i++) {
			for ($j = 0; $j < sizeof($a[$i]); $j++) {
				list($res1,$res2) = sscanf($a[$i][$j], "%d-%d");
				$design[$i+1][$j] = array($res1,$res2); // NOTE: $i+1 to keep the designs 1-based!
			}
		}
		
		return $design;
	} 
}


?>