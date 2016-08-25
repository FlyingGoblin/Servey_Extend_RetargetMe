<?php

/**
 * Common definitions.
 * 
 *  @author Michael Rubinstein, MIT 2009
 */

define('SURVEY_VERSION',2.3);

date_default_timezone_set('America/New_York');
define('DATE_FORMAT',"Y-m-d H:i:s (T)");
define('INVALID',-1);

define('TMP_SESSION_DIR', 'tmp');
define('SESSION_DIR', 'sessions');
define('TEMPLATE_DIR', 'templates');
define('DATA_DIR', 'data');

define('N_DATASETS',15); // number of datasets to choose from (RANDOM mode)
define('N_COMPARISONS',3); // number of comparisons (RANDOM mode)
// question will be displayed with prob' 1/QUESTION_FREQ
define("QUESTION_FREQ_RAND", 24); // RANDOM mode
define("QUESTION_FREQ_EXP", 12); // EXPERIMENT mode

define('HTML_TITLE','Image Retargeting Survey');
 
define('FORM_START', 'startForm');
define('FORM_DETAILS', 'detailsForm');
define('FORM_TRIAL0', 'trial0Form');
define('FORM_TRIAL1', 'trial1Form');
define('FORM_TRIAL2', 'trial2Form');
define('FORM_IMCOMPARE', 'compareForm'); 
define('FORM_IMQUERY', 'queryForm');
define('FORM_END', 'endForm');

define('MODE_RANDOM', 0);
define('MODE_EXP', 1);

// [11/22/2009] added for the second version without the source image
define('EXP_TYPE_NORMAL', 0);
define('EXP_TYPE_NOSRC', 1);

// [12/19/2009] adder support Mechanical Turk users
define('USER_NORMAL', 0);
define('USER_MTURK', 1);

?>