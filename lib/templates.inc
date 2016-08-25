<?php

/**
 * Page templates.
 * The survey system follows a template-based design, which allows for a clean
 * definition, and combination, of customizable HTML code.
 * 
 * @see patTemplate.php
 * @author Michael Rubinstein, MIT 2009
 */

// include the template parser
require_once("patTemplate/patTemplate.php");
require_once("patTemplate/patErrorManager.php");

require_once("lib/conf.inc");

define('TEMPLATE_DEFAULT', 			'default');
define('TEMPLATE_START', 			'start');
define('TEMPLATE_DETAILS', 			'details');
define('TEMPLATE_IMCOMPARE_TRIAL', 'imcompare_trial');
define('TEMPLATE_IMCOMPARE',		'imcompare');
define('TEMPLATE_IMQUERY', 			'imquery');
define('TEMPLATE_IMQUERY_TRIAL', 	'imquery_trial');
define('TEMPLATE_END', 				'end');
define('TEMPLATE_END_UUID', 		'end_uuid');


Class Templates
{	
	static $template = 0;
	static $template_nosrc = 0;
	
	static function &getTemplate($type=NULL) {
		
		if ($type==EXP_TYPE_NORMAL) {
			return self::$template;
		} else if ($type==EXP_TYPE_NOSRC) {
			return self::$template_nosrc;
		}
	}

	
	static function initTemplate() {		

		self::$template = new patTemplate();
		// Set which directory contains the template-files.
		self::$template->setRoot(TEMPLATE_DIR);
		// add templates to the template engine
		self::$template->readTemplatesFromFile("index.tmpl.html");
		self::$template->readTemplatesFromFile("start.tmpl.html");
		self::$template->readTemplatesFromFile("details.tmpl.html");
		self::$template->readTemplatesFromFile("imcompare_trial.tmpl.html");
		self::$template->readTemplatesFromFile("imcompare.tmpl.html");
		self::$template->readTemplatesFromFile("imquery.tmpl.html");
		self::$template->readTemplatesFromFile("imquery_trial.tmpl.html");
		self::$template->readTemplatesFromFile("end.tmpl.html");
		
		// second set of templates for the 'blind' version
		self::$template_nosrc = new patTemplate();
		self::$template_nosrc->setRoot(TEMPLATE_DIR);
		self::$template_nosrc->readTemplatesFromFile("index.tmpl.html");
		self::$template_nosrc->readTemplatesFromFile("details.tmpl.html");
		self::$template_nosrc->readTemplatesFromFile("end.tmpl.html");
		self::$template_nosrc->readTemplatesFromFile("nosrc/start.tmpl.html");
		self::$template_nosrc->readTemplatesFromFile("nosrc/imcompare_trial.tmpl.html");
		self::$template_nosrc->readTemplatesFromFile("nosrc/imcompare.tmpl.html");
		
		
	}
		
	
}

Templates::initTemplate();
								  
?>