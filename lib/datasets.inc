<?php

/**
 * Datasets handling.
 * TODO: specify 'datasets' structure.
 *
 * @author Michael Rubinstein, MIT 2009
 */

include_once('lib/conf.inc');
include_once('lib/utils.inc');

Class Datasets {
	
	public static function updateDatasets() {
		$datasets = self::loadDatasetsFile('data/datasets');
		Utils::serializeToFile($datasets,'bin/datasets.dat');
	}
	
	public static function loadDatasets() {
		return Utils::unserializeFromFile('bin/datasets.dat');
	}
	
	private static function loadDatasetsFile($filename) {
		$a = Utils::dlmread($filename,'|');

		// parse
		$datasets = Array();
		for ($i = 0; $i < sizeof($a); $i++) {
			$did = $a[$i][0];
			$nres = $a[$i][1];
			$attrs = explode(' ',$a[$i][2]);
			$results = Array();
			for ($k = 0; $k < $nres; $k++) {
				$results[$k]=$k+1; 	
			}
			for ($k = 0; $k < count($attrs); $k++) {
				sscanf($attrs[$k],'%d',$attrs[$k]); 	
			}
			$datasets[$did]['results'] = $results;
			$datasets[$did]['attrs'] = $attrs;
			
		}
		
		return $datasets;
	} 
}


?>