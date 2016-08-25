<pre>
<?php

/**
 * Removes all temporary session files, thus clearing any data stored from 
 * previous accesses of the users. Sessions are typically maintained by PHP, and 
 * are cleaned up periodically by the PHP garbage collector (upon running the 
 * interpreter itself?). This script, at any case, gives an explicit way of 
 * clearing the old data.     
 * 
 * @author Michael Rubinstein, MIT 2009
 */

include_once('lib/conf.inc');

$files = glob(TMP_SESSION_DIR . "/*");
$nSessions = count($files);
foreach ($files as $file) {
	// print some session information
	$path_parts = pathinfo($file);
	echo "Session " . $path_parts['basename'] . " from " . date("F d Y H:i:s",filemtime($file)) . "\n";
	unlink($file);
}
echo "Deleted " . $nSessions . " sessions";
?>

</pre>