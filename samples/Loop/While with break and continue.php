<?php
   int $i = 0;
   while(true)
    {
      $i = $i + 1;
       
      if ($i == 20)
          break;
       
      if ($i % 2 == 0)
        continue;

      echo "I = $i";
    }
 ?>
---------------------------------------
I = 1
I = 3
I = 5
I = 7
I = 9
I = 11
I = 13
I = 15
I = 17
I = 19