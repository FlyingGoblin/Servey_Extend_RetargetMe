<?php
include_once("utils.inc");

// we will do our own error handling
define('STDERR_DEVICE_FILE',1);
define('STDERR_DEVICE_BROWSER',2);
define('STDERR_DEVICE_DB',3);
define('STDERR_DEVICE', STDERR_DEVICE_FILE);
error_reporting(E_ALL); // report all errors

$prev_error_msg = "";
// user defined error handling function
function userErrorHandler($errno, $errmsg, $filename, $linenum, $vars)
{
	global $prev_error_msg, $database;	
	// timestamp for the error entry
	$dt = date("Y-m-d H:i:s (T)");	
	if (strpos($filename, 'patTemplate') > -1 || strpos($filename, 'simpletest') > -1)
	{
		return; // don't log patTemplate errors.
	}
	// define an assoc array of error string
	// in reality the only entries we should
	// consider are E_WARNING, E_NOTICE, E_USER_ERROR,
	// E_USER_WARNING and E_USER_NOTICE
	$errortype = array (
		E_ERROR				=> "Error",
		E_WARNING			=> "Warning",
		E_PARSE				=> "Parsing Error",
		E_NOTICE			=> "Notice",
		E_CORE_ERROR		=> "Core Error",
		E_CORE_WARNING		=> "Core Warning",
		E_COMPILE_ERROR		=> "Compile Error",
		E_COMPILE_WARNING	=> "Compile Warning",
		E_USER_ERROR		=> "User Error",
		E_USER_WARNING		=> "User Warning",
		E_USER_NOTICE		=> "User Notice"
		//E_STRICT          => "Runtime Notice" // undefined in PHP4
	);
	/*if (strpos($errortype[$errno], "Notice") > -1)
	{
	return; // don't log notices.
	}*/
	// set of errors for which a var trace will be saved
	$user_errors = array(E_USER_ERROR, E_USER_WARNING, E_USER_NOTICE);

	// ignore these known errors and notices
	/*if (strpos($errmsg, 'var: ') === 0 // using var as in PHP4 is ok
		|| strpos($errmsg, 'should not be called statically') > -1	
		|| strpos($errmsg, 'is_a(): Deprecated') > -1 // IN PHP4 is ok.
		|| strpos($errmsg, 'IS_NOT_AJAX') > -1 // IS_NOT_AJAX is not defined
		|| strpos($errmsg, 'It is not safe') > -1 // timezone issues		
	)	
	{
		return;
	}*/
	if (strpos($errmsg, 'pg_exec') > -1 || strpos($errmsg, 'pg_query') > -1)
	{
		str_replace("\n", "\r\n", $errmsg);
		$append_command = true;
	}
	else $append_command = false;

	$err = "<BR><u>Error Entry</u><pre>\r\n";
	$err .= "\tdatetime: " . $dt . "\r\n";
	$err .= "\tnum:" . $errno . "\r\n";
	if (key_exists($errno, $errortype)) {
		$error_type = $errortype[$errno];
	} else $error_type = 'UNKNOWN ERROR NO'  . $errno;
	$err .= "\ttype: " . $error_type . "\r\n";
	$err .= "\tmsg: " . $errmsg . "\r\n";
	if ($append_command) // append the last command we tried to exec.
		$err .= "\tcommand: " . $database->last_command . "\r\n";
	$err .= "\tscript name: " . $filename . "\r\n";
	$err .= "\tline: " . $linenum . "\r\n";
	$err .= "trace:</pre>\r\n" . debug_getBackTrace() . "\r\n\r\n";

	if ($err == $prev_error_msg)
	{
		return;
	}
	$prev_error_msg = $err;

	outputError($err);		
}

function outputError($err, $filename = "error_log.txt")
{
	global $database;
	global $objectResponse;
	// for testing
	// echo $err;
	$err .= "\r\n";
	switch (STDERR_DEVICE)
	{
		case STDERR_DEVICE_FILE:			
			Utils::writeToFile($filename, $err);
			break;
		case STDERR_DEVICE_DB:			
			$database->insertError("no SQL command", $err);
		break;
	}
}

// taken from some comment in the php manual
function debug_getBackTrace()
{
	$s = '';
	$MAXSTRLEN = 64;
	
	$s = '<pre align="left">';
	$traceArr = debug_backtrace();
	array_shift($traceArr);
	$tabs = sizeof($traceArr)-1;
	foreach($traceArr as $arr)
	{
		for ($i=0; $i < $tabs; $i++) $s .= ' &nbsp; ';
		$tabs -= 1;
		$s .= '<font face="Courier New,Courier">';
		if (isset($arr['class'])) $s .= $arr['class'].'.';
		$args = array();
		if (!empty($arr['args'])) 
			foreach($arr['args'] as $v)
			{
				if (is_null($v)) $args[] = 'null';
				else if (is_array($v)) $args[] = 'Array['.sizeof($v).']';
				else if (is_object($v)) $args[] = 'Object:'.get_class($v);
				else if (is_bool($v)) $args[] = $v ? 'true' : 'false';
				else
				{ 
					$v = (string) @$v;
					$str = htmlspecialchars(substr($v,0,$MAXSTRLEN));
					if (strlen($v) > $MAXSTRLEN) 
						$str .= '...';
					$args[] = "\"".$str."\"";
				}
			}
		$s .= $arr['function'].'('.implode(', ',$args).')</font>';
		$Line = (isset($arr['line'])? $arr['line'] : "unknown");
		$File = (isset($arr['file'])? $arr['file'] : "unknown");
		$s .= sprintf("<font color=#808080 size=\"-1\"> # line %4d, file: <a href=\"file:/%s\">%s</a></font>",
			$Line, $File, $File);
		$s .= "\n";
	}    
	$s .= '</pre>';
	return $s;
} 
// set the error handler to the usr's handler.
$old_error_handler = set_error_handler("userErrorHandler");
?>