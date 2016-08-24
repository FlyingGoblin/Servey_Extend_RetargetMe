<?php

/**
 * Updates the survey data on the server. 
 * This script should be executed upon starting a new survey, or whenever there 
 * were changes to either the experiment design, questions (text, etc) or the 
 * dataset. It parses the relevant data and serializes it to a PHP format that 
 * is then easy to load and use during run time.
 * 
 * @author Michael Rubinstein, MIT 2009
 */

include_once('lib/conf.inc');
include_once('lib/utils.inc');
include_once('lib/designs.inc');
include_once('lib/experiments.inc');
include_once('lib/attributes.inc');
include_once('lib/datasets.inc');


echo "Processing...<br>";

Designs::updateDesigns();
Experiments::updateExperiments();
Attributes::updateQuestions();
Datasets::updateDatasets();


echo "Update Complete! (version=" . SURVEY_VERSION . ")";



