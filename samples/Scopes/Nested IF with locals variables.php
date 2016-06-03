<?php

  int $x = 1;
  echo $x;
  if (true)
  {
    int $x = 10;
    echo $x;
    if (true){
      int $x = 100;
      echo $x;
      
      if (true){
        int $x = 1000;
        echo $x;
      }
      echo $x;
    }
    echo $x;
  }
  echo $x;
 ?>
-----------------------------------------------------
1
10
100
1000
100
10
1