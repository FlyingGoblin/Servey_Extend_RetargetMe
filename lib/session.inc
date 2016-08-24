<?php

/**
 * User session handling.
 * 
 * @author Michael Rubinstein, MIT 2009
 */

define('PARAM_STATE', 'state');
define('PARAM_USERNAME','username');
define('PARAM_PATH_STEP','path_step');
define('PARAM_COMPARISON_PATH','comprison_path');
define('PARAM_METADATA','metadata');
define('PARAM_PAGE', 'page');
define('PARAM_MODE','mode');
define('PARAM_EXPID','expid');
define('PARAM_EXP_TYPE','expType');
define('PARAM_PAGEID','pageid');
define('PARAM_LAST_SELECTED_ID','selected_id');
define('PARAM_USER','user');


include_once('lib/conf.inc');
include_once('lib/utils.inc');

// A session object maintains all relevant user information on the server
class Session {
	
	function Session() {
		// for now point temporary sessions to our path, but need to check
		// if gc works on that as well (gc is probabilistic and invoked on session
		// starts)
		session_save_path(TMP_SESSION_DIR);
		try {
			session_start(); // load session
		} catch (Exception $ex)	{
			outputError($ex);	
		}
	}
	
	public function getSessionFile() {
		return SESSION_DIR . "/" . session_id() . '.dat';
	}
	
	public function getSessionId() {
		return session_id();
	}
	
	public function isInitialized() {
		return key_exists(PARAM_PAGEID, $_SESSION); // that's one way to tell...
	}
	
	public function setPageId($pageid) {
		$_SESSION[PARAM_PAGEID] = $pageid;
	}
	
	public function getPageId() {
		return $_SESSION[PARAM_PAGEID];
	}
	
	public function incrementPage() {
		$this->setPageId($this->getPageId()+1);
	}
	
	public function setPathStep($step) {
		$_SESSION[PARAM_PATH_STEP] = $step;
	}
	
	public function getPathStep() {
		return $_SESSION[PARAM_PATH_STEP];
	}
	
	public function setLastSelectedId($selected_id) {
		$_SESSION[PARAM_LAST_SELECTED_ID] = $selected_id;	
	}
	
	public function getLastSelectedId() {
		return $_SESSION[PARAM_LAST_SELECTED_ID];
	}
	
	// Stores this comparison path in the session
	public function setComparisonPath($path) {
		$_SESSION[PARAM_COMPARISON_PATH] = $path;
		$this->setPathStep(0);
	}
	
	// Reads the comparison path stored in this session
	public function getComparisonPath() {
		return $_SESSION[PARAM_COMPARISON_PATH];
	}
	
	public function getComparisonPathLength() {
		return count($this->getComparisonPath());
	}
	
	// keeps track of the user progress in the survey
	public function getState() {
		return $_SESSION[PARAM_STATE];	
	}
	
	public function setState($state) {
		$_SESSION[PARAM_STATE] = $state;
	}
	
	public function getMode() {
		return $_SESSION[PARAM_MODE];
	}
	
	public function setMode($mode) {
		$_SESSION[PARAM_MODE] = $mode;
	}
	
	public function getExperimentId() {
		return $_SESSION[PARAM_EXPID];
	}
	
	public function setExperimentId($expid) {
		$_SESSION[PARAM_EXPID] = $expid;
	}
	
	public function getExperimentType() {
		return $_SESSION[PARAM_EXP_TYPE];
	}
	
	public function setExperimentType($expType) {
		$_SESSION[PARAM_EXP_TYPE] = $expType;
	}
	
	public function getUser() {
		return $_SESSION[PARAM_USER];
	}
	
	public function setUser($user) {
		$_SESSION[PARAM_USER] = $user;
	}
	
	public function init($mode,$expid,$expType,$user) {
		$this->setMode($mode);
		$this->setExperimentId($expid);
		$this->setExperimentType($expType);
		$this->setUser($user);
		
		$this->setPageId(0);
	}
	
	

}

$session = new Session(); // global ==> happens every time a page loads
?>