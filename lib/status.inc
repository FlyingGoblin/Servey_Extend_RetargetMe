<?php

include_once('lib/conf.inc');
include_once('lib/utils.inc');

Class Status {
	
	public static function updateVersion() {
		$ver = self::loadVersionFile('data/version');
		Utils::serializeToFile($ver,'bin/version.dat');
	}
	
	public static function loadVersion() {
		return Utils::unserializeFromFile('bin/version.dat');
	}
	
	private static function loadVersionFile($filename) {
		$a = Utils::dlmread($filename,' ');

		sscanf($a[0][0],'%f',$ver);
		return $ver;
	} 
}


?>