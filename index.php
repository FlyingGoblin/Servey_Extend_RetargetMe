<?php

/**
 * The survey entry point and state machine.
 * 
 * @author Michael Rubinstein, MIT 2009
 */

include_once('lib/conf.inc');
include_once('lib/error_handling.inc');
include_once('lib/utils.inc');
include_once('lib/session.inc');
include_once('lib/templates.inc');
include_once('lib/datasets.inc');
include_once('lib/experiments.inc');
include_once('lib/designs.inc');
include_once('lib/plan.inc');
include_once('lib/attributes.inc');


if (!$session->isInitialized()) {
	$mode = (key_exists("mode",$_GET) ? $_GET['mode'] : INVALID);
	$expid = (key_exists("expid",$_GET) ? $_GET['expid'] : INVALID);
	$expType = (key_exists("exptype",$_GET) ? $_GET['exptype'] : EXP_TYPE_NORMAL); // default to the 'normal' survey
	$user = (key_exists("user",$_GET) ? $_GET['user'] : USER_NORMAL); // default to the 'normal' user
	if ($mode==INVALID && $expid==INVALID) {
//		die("Please use the original link that was provided to you."); // TODO: verify other attributes as well
		$mode = 1;
	}
	$session->init($mode,$expid,$expType,$user);
	$session->setState('start');
	$session->incrementPage();
}

$sessionFile = $session->getSessionFile();

// get the name of the form being submitted (if at all)
$formname = NULL; $pageid = NULL;
if (key_exists('formname', $_REQUEST)) {
	$formname = $_REQUEST['formname'];
	$pageid = $_REQUEST['pageid'];
}

// initialize templates based on experiment type
$template = &Templates::getTemplate($session->getExperimentType());

/**
 * Process submitted user data, and update the session file.
 * NOTE: check if this is a newly submitted form to avoid reprocessing on
 * user refresh/back etc. Otherwise - nothing changes, so just present the next page
 */
if (isset($formname) && $pageid >= $session->getPageId()) {
	if ($formname == FORM_START) {
		$mode = $session->getMode();
		$expid = $session->getExperimentId();
		$expType = $session->getExperimentType();
		$user = $session->getUser();
		if ($mode == 0) { // random
			$datasets = Datasets::loadDatasets();
			$questionFreq = $expType==EXP_TYPE_NORMAL ? QUESTION_FREQ_EXP : 0; // HACK: we want extra questions only in normal mode
			$path = generateRandomPath($datasets,N_DATASETS,N_COMPARISONS,$questionFreq);
		} else {
			$experiments = Experiments::loadExperiments();
			$designs = Designs::loadDesigns();
			if ($mode == 1) { // next experiment
				$expid = Experiments::checkout(INVALID, $expType);
			} else {
				$expid = Experiments::checkout($expid, $expType);
			}
			if ($expid==INVALID) {
				die('The survey webpage is currently busy. Please try again later...');
			}
			$session->setExperimentId($expid);
			$experiment = $experiments[$expid];
			$questionFreq = $expType==EXP_TYPE_NORMAL ? QUESTION_FREQ_EXP : 0; // HACK: we want extra questions only in normal mode
			$path = generateExperimentPath($experiment,$designs,$questionFreq);
		}
		$session->setComparisonPath($path);
		Utils::writeToFile($sessionFile, "\r\n# New session started: " . date(DATE_FORMAT));
		Utils::writeToFile($sessionFile, "\r\n# mode=" . $mode . ", expid=" . $expid . ", expType=" . $expType . ", user=" . $user);
		Utils::writeToFile($sessionFile, "\r\n" . SURVEY_VERSION);
		$client_ip = $_SERVER['REMOTE_ADDR'];
		$client_host = gethostbyaddr($client_ip);
		$client_browser = $_SERVER['HTTP_USER_AGENT'];
		$record = "$client_ip|$client_host|$client_browser";
		Utils::writeToFile($sessionFile, "\r\n" . $record);
		$session->setState('details');
		$session->incrementPage();
	} else if ($formname == FORM_DETAILS) {
		$gender = $_REQUEST['gender'];
		$age = $_REQUEST['age'];
		$gpx_bkgrnd = $_REQUEST['gpx_bkgrnd'];
		$record = "$gender|$age|$gpx_bkgrnd";
		Utils::writeToFile($sessionFile, "\r\n" . $record);
		$session->setState('trial0');
		$session->incrementPage();
	} else if ($formname == FORM_TRIAL0) { // nothing to store during trials, so just update the state
		$session->setState('trial1');
		$session->incrementPage();
	} else if ($formname == FORM_TRIAL1) {
		if ($session->getExperimentType()==EXP_TYPE_NORMAL) { 
			$session->setState('trial2');
		} else { // shorter trial
			$session->setState('compare');
		}
		$session->incrementPage();		
	} else if ($formname == FORM_TRIAL2) {
		$session->setState('compare');
		$session->incrementPage();		
	} else if ($formname == FORM_IMCOMPARE) {
		$dataset_id = $_REQUEST['dataset_id'];
		$res1_id = $_REQUEST['res1_id'];
		$res2_id = $_REQUEST['res2_id'];
		$selected_id = $_REQUEST['selected_id'];
		$path_step = $_REQUEST['path_step'];
		$time = $_REQUEST['time'];
		echo in_array(1, $GLOBALS['CHECKPOINT_LIST']);
		if(in_array($dataset_id, $GLOBALS['CHECKPOINT_LIST'])){
			if($selected_id == $GLOBALS['CHECKPOINT_LIST_BETTER'][$dataset_id]){
				$record = "CHECKPOINT|PASS";
			} else{
				$record = "CHECKPOINT|FAIL";
			}
		} else{
			$record = "$path_step|$dataset_id|$res1_id|$res2_id|$selected_id|$time";
		}
		Utils::writeToFile($sessionFile, "\r\n" . $record);
		$session->setLastSelectedId($selected_id);	
		
		$path = $session->getComparisonPath();
		$shouldQuery = $path[$path_step][3];
		if ($shouldQuery) {
			$session->setState('query');
		} else {
			$session->setPathStep($path_step+1);
		}
		$session->incrementPage();
	} else if ($formname == FORM_IMQUERY) {
		$selected_id = $_REQUEST['selected_id'];
		$time = $_REQUEST['time'];
		Utils::writeToFile($sessionFile, "|$selected_id|$time");
		$path_step = $_REQUEST['path_step'];
		$session->setPathStep($path_step+1);
		$session->setState('compare');
		$session->incrementPage();
	}
	
	// finally check if we're done
	if ($session->getPathStep() == $session->getComparisonPathLength()) {
		Utils::writeToFile($sessionFile, "\r\n# Session ended successfully: " . date(DATE_FORMAT) . "\r\n");
		$session->setState('last');		
	
		// if in experiment mode - release it
		$expid = $session->getExperimentId();
		if ($expid!=INVALID) {
			Experiments::checkin($expid,$session->getExperimentType());
		}
	}
	
}

/**
 * This is the main survey state machine.
 * Based on current state, it prepares the next page and sends it to the user
 */

$state = $session->getState();

if ($state == 'start') { // $ start page
	$template->addVar(TEMPLATE_START, 'PAGEID', $session->getPageId());
	$template->addVar(TEMPLATE_START, 'FORM_NAME', FORM_START);
	$content = $template->getParsedTemplate(TEMPLATE_START);
} else if ($state == 'details') { // user details page
	$template->addVar(TEMPLATE_DETAILS, 'PAGEID', $session->getPageId());
	$template->addVar(TEMPLATE_DETAILS, 'FORM_NAME', FORM_DETAILS);
	$content = $template->getParsedTemplate(TEMPLATE_DETAILS);
} else if ($state == 'trial0') { // trial comparison
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'DATASET_ID', 'trial1');
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'RES1_ID', 1);
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'RES2_ID', 2);
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'EXPECTED_ID', 2);
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'TRIALNO', 0);
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'PAGEID', $session->getPageId());
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'FORM_NAME', FORM_TRIAL0);
	$content = $template->getParsedTemplate(TEMPLATE_IMCOMPARE_TRIAL);
} else if ($state == 'trial1') { // trial comparison
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'DATASET_ID', 'trial2');
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'RES1_ID', 1);
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'RES2_ID', 2);
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'EXPECTED_ID', 1);
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'TRIALNO', 1);
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'PAGEID', $session->getPageId());
	$template->addVar(TEMPLATE_IMCOMPARE_TRIAL, 'FORM_NAME', FORM_TRIAL1);
	$content = $template->getParsedTemplate(TEMPLATE_IMCOMPARE_TRIAL);
} else if ($state == 'trial2') { // trial comparison
	$datasets = Datasets::loadDatasets();
	$questions = Attributes::loadQuestions();

	$values = $datasets[61]['attrs']; // HACK: dataset 61 corresponds to trial2
	for ($i = 0; $i < count($values); $i++) {
		$options[$i] = $questions[$values[$i]];
	}
	
	$template->addVar(TEMPLATE_IMQUERY_TRIAL, 'DATASET_ID', 'trial2');
	$template->addVar(TEMPLATE_IMQUERY_TRIAL, 'RES_ID', 2); // cause 1 must be chosen
	$template->addVar('entry_trial', 'OPTIONS', $options);
	$template->addVar('entry_trial', 'VALUES', $values);
	$template->addVar(TEMPLATE_IMQUERY_TRIAL, 'PAGEID', $session->getPageId());
	$template->addVar(TEMPLATE_IMQUERY_TRIAL, 'FORM_NAME', FORM_TRIAL2);
	$content = $template->getParsedTemplate(TEMPLATE_IMQUERY_TRIAL);
} else if ($state == 'compare' || $state == 'query') {
	$path_step = $session->getPathStep();
	$path = $session->getComparisonPath();
				
	$comparison = $path[$path_step];
	$dataset_id = $comparison[0];
	$res1_id = $comparison[1]; 
	$res2_id = $comparison[2];
	
	$page = $path_step+1;
	$npages = count($path);
	$progress = ($page/$npages)*100.0;
	
	if ($state == 'compare') {  // image comparison page
		$template->addVar(TEMPLATE_IMCOMPARE, 'DATASET_ID', $dataset_id);
		$template->addVar(TEMPLATE_IMCOMPARE, 'RES1_ID', $res1_id);
		$template->addVar(TEMPLATE_IMCOMPARE, 'RES2_ID', $res2_id);
		$template->addVar(TEMPLATE_IMCOMPARE, 'PATH_STEP', $path_step);
		$template->addVar(TEMPLATE_IMCOMPARE, 'PAGEID', $session->getPageId());
		$template->addVar(TEMPLATE_IMCOMPARE, 'FORM_NAME', FORM_IMCOMPARE);	
	
		$template->addVar(TEMPLATE_IMCOMPARE, 'PAGE', $page);
		$template->addVar(TEMPLATE_IMCOMPARE, 'NPAGES', $npages);
		$template->addVar(TEMPLATE_IMCOMPARE, 'PROGRESS', $progress);
		
		$content = $template->getParsedTemplate(TEMPLATE_IMCOMPARE);
	} else { // query page
		$selected_id = $session->getLastSelectedId();
		$res_id = ($selected_id == $res1_id ? $res2_id : $res1_id);
		$datasets = Datasets::loadDatasets();
		$questions = Attributes::loadQuestions();

		$values = $datasets[$dataset_id]['attrs'];
		for ($i = 0; $i < count($values); $i++) {
			$options[$i] = $questions[$values[$i]];
		}
		
		$template->addVar(TEMPLATE_IMQUERY, 'DATASET_ID', $dataset_id);
		$template->addVar(TEMPLATE_IMQUERY, 'RES_ID', $res_id);
		$template->addVar('entry', 'OPTIONS', $options);
		$template->addVar('entry', 'VALUES', $values);
		$template->addVar(TEMPLATE_IMQUERY, 'PATH_STEP', $path_step);
		$template->addVar(TEMPLATE_IMQUERY, 'PAGEID', $session->getPageId());
		$template->addVar(TEMPLATE_IMQUERY, 'FORM_NAME', FORM_IMQUERY);
		
		$template->addVar(TEMPLATE_IMQUERY, 'PAGE', $page);
		$template->addVar(TEMPLATE_IMQUERY, 'NPAGES', $npages);
		$template->addVar(TEMPLATE_IMQUERY, 'PROGRESS', $progress);
	
		$content = $template->getParsedTemplate(TEMPLATE_IMQUERY);
	} 
} else if ($state == 'last') {
	$template->addVar(TEMPLATE_END, 'PAGEID', $session->getPageId());
	$template->addVar(TEMPLATE_END, 'FORM_NAME', FORM_END);
	if ($session->getUser() == USER_MTURK) {
		$template->addVar(TEMPLATE_END_UUID, 'UUID', $session->getSessionId());
	}
	$content = $template->getParsedTemplate(TEMPLATE_END);
	session_unset();
	session_destroy();
}

// prepare the page and send to the user
$template->addVar(TEMPLATE_DEFAULT, 'HTML_TITLE', HTML_TITLE);
$template->addVar(TEMPLATE_DEFAULT, 'CONTENT', $content);
$template->displayParsedTemplate(TEMPLATE_DEFAULT);

?>
