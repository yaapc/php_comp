<?php

  require_once('Integer.php');
  
 
  Integer $zero = new Integer();
  Integer $ten  = new Integer(10);
  
  echo $zero;
  echo $ten;
  
  string $zeroString = $zero->stringValue();
  string $tenString  = $ten->stringValue();
  
  echo $zeroString;
  echo $tenString;
  
  float $zeroFloat  = $zero->floatValue();
  float $tenFloat   = $ten->floatValue();
  
  echo $zeroFloat;
  echo $tenFloat;
?>

