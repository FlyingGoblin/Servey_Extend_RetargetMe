<?php

/**
 * Common utility functions.
 * 
 * @author Michael Rubinstein, MIT 2009
 */

Class Utils {
	
	public static function q($str) {
		return "'" . $str . "'";
	}
	
	public static function qq($str) {
		return "\"" . $str . "\"";
	}
	
	public static function escapeSQL($str) {
		//return urlencode($str);
		return str_replace("'","\'",$str);
	}
	
	public static function hex2bin($hex) {
		return pack("H*", $hex); 
	}
	
	public static function safeUTF8($str) {
		$hex = bin2hex($str);
		$hex = str_replace("e2803f","", $hex);
		return Utils::hex2bin($hex);
	}
	
	public static function writeToFile($filename, $data, $mode = 'a')
	{
		$file_handle = fopen($filename,$mode);
		if ($data == null) {
			fwrite($file_handle, 'NULL');
		} else if (!is_array($data)) { // write as is.
			fwrite($file_handle, $data);
		} else {
			fwrite($file_handle, "Array: \r\n");
			foreach ($data as $k=>$v) {
				fwrite($file_handle, "\t$k=>$v\r\n");
			}
		}
		fclose($file_handle);
	}
	
	public static function serializeToFile($a,$filename) {
		$serialized = serialize($a);
		self::writeToFile($filename, $serialized, 'w');
	}
		
	public static function readFile($filename) {
		$content = file_get_contents($filename);
		return $content;		
	}
	
	public static function unserializeFromFile($filename) {
		$serialized = self::readFile($filename);
		return unserialize($serialized);
	}	

	public static function getDirectory($path = '.', $ignore = '', $subdir = false) {
	    $dirTree = array ();
	    $dirTreeTemp = array ();
	    $ignore[] = '.';
	    $ignore[] = '..';
	
	    $dh = @opendir($path);
	
	    while (false !== ($file = readdir($dh))) {
	        if (!in_array($file, $ignore)) {
	            if (!is_dir("$path/$file")) {
	            	if ($subdir) {
	            		$arr = explode('/', $path);         		
	            		$s = end($arr);
	            		$dirTree["$s"][] = $file;
	            	} else {
		                $dirTree["$path"][] = $file;
	            	}
	            } else {
	                $dirTreeTemp = self::getDirectory("$path/$file", $ignore, $subdir);
	                if (is_array($dirTreeTemp)) $dirTree = $dirTree + $dirTreeTemp;
	            }
	        }
	    }
	    closedir($dh);
	    
	    return $dirTree;
	}
	
	public static function dlmread($filename,$delim) {
		$handle = fopen($filename, "r");
		$a = Array();
		if ($handle) {
			while(!feof($handle)) {
				$line = trim(fgets($handle));
		        if (strlen($line) > 0) {
		    		$a[] = explode($delim,$line);
		        }
			}  
		}
		return $a;
	}
	
	public static function fisherYatesShuffle(&$array)
	{
   		for ($i = count($array); --$i; $i > 0)
	   {
	      	$j = @mt_rand(0, $i);
	       	$tmp = $array[$i];
	        $array[$i] = $array[$j];
	        $array[$j] = $tmp;
	   }
	}
 
}
?>