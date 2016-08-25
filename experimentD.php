<?php
/**
 * Created by PhpStorm.
 * User: ChenYang
 * Date: 2016/8/23
 * Time: 14:39
 */
include_once('lib/utils.inc');

$did_list = array(4, 50, 11, 12, 14, 21, 23, 24, 28, 29, 31, 35, 39, 45, 49, 5, 51, 61, 64, 68, 69, 70, 74, 78, 82, 84, 86, 88, 89);
$image_size = count($did_list);
echo count($did_list);

$my_experiments = array();
$my_experiments_status = array();

$begin = 0;
$count = 1;
while($begin<$image_size){
    if($begin<20){
        $experiment_size = 4;
    }else{
        $experiment_size = 3;
    }
    for($i = 0; $i<7; $i++){
        $experiment = array();
        for($j = 0; $j<$experiment_size; $j++){
            $sub_experiment = array($did_list[$begin+$j], 8, $i+1);
            $experiment[$j] = $sub_experiment;
            echo print_r($sub_experiment, 1).'<br>';
        }
        $my_experiments[$count] = $experiment;
        $my_experiments_status[$count] = array('count'=>0, 'time'=>0);
        $count++;
    }
    $begin += $experiment_size;
}
echo count($my_experiments);

Utils::serializeToFile($my_experiments,'bin/experiments_4img.dat');
Utils::serializeToFile($my_experiments_status,'bin/experiments_4img_nosrc_status.dat');
Utils::serializeToFile($my_experiments_status,'bin/experiments_4img_status.dat');
?>