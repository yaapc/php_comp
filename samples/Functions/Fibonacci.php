<?php
  function fib(int $n): int {
    if ($n == 0){
       return 0;
    }
		if($n == 1) {
			return 1;
		}
		return fib($n - 1) + fib($n -2);
  }
  echo fib(14);
?>
----------------------------------------------------
377