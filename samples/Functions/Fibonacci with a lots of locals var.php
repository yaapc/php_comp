<?php
  function fib(int $n): int
  {
      if ($n == 0)
      {
         int $n1 =11;
         return 0;
      }
		  if($n == 1)
      {
         int $n1 =54;
         if (true){
            int $n1 = 100;
         }
			    return 1;
		  }

      if (true)
        int $n1 = 40;


		  return fib($n - 1) + fib($n -2);
  }
  echo fib(31);
?>
---------------------------------------------------
89
Note: without optimizaton (unless you fix the problems).
