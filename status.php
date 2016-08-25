<?php

/**
 * Creates an experiment status report.
 * 
 * @author Michael Rubinstein, MIT 2009
 */

include_once('lib/conf.inc');
include_once('lib/experiments.inc');


$exp=Experiments::loadExperimentsStatus(EXP_STATUS_FILE);
printf("EXPERIMENTS STATUS:<br>");
foreach($exp as $expid => $value) {
	$date_str = ($value['time'] > 0 ? date(DATE_FORMAT,$value['time']) : '');
	printf("%d | %d | %s<br>",$expid,$value['count'],$date_str);
}

$exp=Experiments::loadExperimentsStatus(EXP_NOSRC_STATUS_FILE);
printf("EXPERIMENTS NOSRC STATUS:<br>");
foreach($exp as $expid => $value) {
	$date_str = ($value['time'] > 0 ? date(DATE_FORMAT,$value['time']) : '');
	printf("%d | %d | %s<br>",$expid,$value['count'],$date_str);
}

?>