<?php

  int $x = 10;
  int $i = 0;
    
   while(true)
    {
      $i = $i + 1;
      int $x = 100;
      if ($i == 20)
          break;
       
      if ($i % 2 == 0){
        int $x = 1000;
        echo $x;
        continue;
      }

      echo $x;
      echo "I = $i";
    }
    
    echo $x;
 
 ?>