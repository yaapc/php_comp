<?php

  string $s1 = "HaHa";
  string $s2 = "HeHe";
  string $s3;
  
  echo $s1," ",$s2;
  
  echo "Initial value is |",$s3,"|";
  
  $s3 = $s1 + "|" + $s2;
  
  echo $s3;
  
  int $x = 1500;
  
  $s3 +=  $x + $x;
  
  echo $s3;
  
?>