<?php

/**
 * Survey generation (plans) for random and designed experiment.
 * TODO: specify 'path' structure
 * 
 * @author Michael Rubinstein, MIT 2009
 */

/*
function generateRandomPath($datasets,$ndatasets,$ncomparisons,$qfreq) {
	$selected_datasets = getRandomSubArray(array_keys($datasets), $ndatasets);
	// will hold comparison information in the following structure:
	// [dataset_id, res1_id, res2_id, query?]
	$path = Array();
	// go over datasets, select a few pairs of images.
	foreach ($selected_datasets as $dataset_id) {
		$results = array_values($datasets[$dataset_id]['results']);

		// get a random subset of the possible relations between the results
		$available_comparisons = getPairsAsymmetric($results);
		$selected_comparisons = getRandomSubArray($available_comparisons, $ncomparisons);
		foreach ($selected_comparisons as $index=>$pair) { // prepend the dataset id
			$selected_comparisons[$index] = array($dataset_id, $pair[0], $pair[1]);
		}	
		$path = array_merge($path, $selected_comparisons);
	}			
	
	// check for which comparison we would also query on the result
	for ($i=0; $i<count($path); $i++) {
		$path[$i][3] = (@mt_rand(1, $qfreq) == 1) ? true : false;
	}
	
	myShuffle($path);

	return $path;
}
*/

function generateRandomPath($datasets,$ndatasets,$nresults,$qfreq) {
	$selected_datasets = getRandomSubArray(array_keys($datasets), $ndatasets);
	// will hold comparison information in the following structure:
	// [dataset_id, res1_id, res2_id, query?]
	$path = Array();
	// go over datasets, select a few pairs of images.
	foreach ($selected_datasets as $dataset_id) {
		$results = array_values($datasets[$dataset_id]['results']);
		$selected_results = getRandomSubArray($results,$nresults);

		$selected_comparisons = getPairsAsymmetric($selected_results);
		foreach ($selected_comparisons as $index=>$pair) { // prepend the dataset id
			$selected_comparisons[$index] = array($dataset_id, $pair[0], $pair[1]);
		}	
		$path = array_merge($path, $selected_comparisons);
	}			
	
	// check for which comparison we would also query on the result
	for ($i=0; $i<count($path); $i++) {
		if ($qfreq != 0) {
			$path[$i][3] = (@mt_rand(1, $qfreq) == 1) ? true : false;	
		} else {
			$path[$i][3] = false;
		}
	}
	
	myShuffle($path);

	return $path;
}

// [8/21/2016] add checkpoint
define('CHECKPOINT1', 101);
define('CHECKPOINT2', 102);
define('CHECKPOINT3', 103);
$CHECKPOINT_LIST=array(CHECKPOINT1, CHECKPOINT2, CHECKPOINT3);
$CHECKPOINT_LIST_BETTER=array(CHECKPOINT1 => 1, CHECKPOINT2 => 2, CHECKPOINT3 => 1);

function generateExperimentPath($experiment,$designs,$qfreq) {
	$path = Array();
	
	for ($i = 0; $i < count($experiment); $i++) {
//	for ($i = 0; $i < 1; $i++) {
		$datasetid = $experiment[$i][0];
		$designid = $experiment[$i][1];
		$judgeid = $experiment[$i][2];
		$comparisons = $designs[$designid][$judgeid];
		for ($j = 0; $j < count($comparisons); $j++) {
			$comparison = $comparisons[$j];
			$comparison = array($datasetid, $comparison[0], $comparison[1]);
			$flip = (@mt_rand(1, 2) == 1) ? true : false; // 50% flip
			if ($flip) {
			 	$tmp = $comparison[1];
			 	$comparison[1] = $comparison[2];
			 	$comparison[2] = $tmp;
			}
			if ($qfreq != 0) {
				$comparison[3] = (@mt_rand(1, $qfreq) == 1) ? 1 : 0;
			} else {
				$comparison[3] = 0;
			}
			$path[] = $comparison; 
		}
	}

	// add checkpoint
	foreach ( $GLOBALS ['CHECKPOINT_LIST'] as $checkpoint ){
		$comparison = array($checkpoint, 1, 2, 0);
		$path[] = $comparison;
	}

	myShuffle($path);
	
	return $path;
}

function getRandomSubArray($array, $length) {
	if (count($array) < $length) { // might be redundant... 
		$length = count($array); 
	}
	myShuffle($array);
	return array_slice($array, 0, $length);
}

// Generates list of (|a| choose 2) pairs
function getPairsAsymmetric($a) {
	$result = array();
	for ($i=0; $i<count($a); $i++) {
		for ($j=$i+1; $j<count($a); $j++) {
			$b = array($a[$i], $a[$j]);
			$flip = (@mt_rand(1, 2) == 1) ? true : false; // 50% flip
			if ($flip) {
				$tmp = $b[0];
				$b[0] = $b[1];
				$b[1] = $tmp;						
			}
			$result[] = $b;
		}
	}
	return $result;
}

function myShuffle(&$array) {
	shuffle($array);
	//Utils::fisherYatesShuffle($comparisons);
	
}


?>