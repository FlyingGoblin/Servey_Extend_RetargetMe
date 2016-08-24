<?php

/**
 * Image attributes handling (currently used for determining the options that 
 * should be given to the user in the additional questions).
 * TODO: specify 'quest' structure
 * 
 * @author Michael Rubinstein, MIT 2009
 */

include_once('lib/conf.inc');
include_once('lib/utils.inc');

Class Attributes {
	
	public static function updateQuestions() {
		$quest = self::loadQuestionsFile('data/questions');
		Utils::serializeToFile($quest,'bin/questions.dat');
	}
	
	public static function loadQuestions() {
		return Utils::unserializeFromFile('bin/questions.dat');
	}
	
	private static function loadQuestionsFile($filename) {
		$a = Utils::dlmread($filename,'|');
		
		$quest = Array();
		for ($i = 0; $i < sizeof($a); $i++) {
			list($qid,$question) = $a[$i];
			$quest[$qid] = $question;
		}
		return $quest;
	} 
}


?>