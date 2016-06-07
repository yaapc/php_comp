<?php

	function fun(int $x) : int {

    if (true)
    {
      echo "Hi" + " A i am" + " an optimized Function";
      return 0;
    }
    else
    {
      echo "This will not be in AST";
      return 0;
   }

    while(false){
      echo "This will not be in AST";
    }

	  while(true)
    {
		  echo "Hi" + " A i am" + " an optimized Function";
		  break;
		  echo "This will not be in AST";
	}

    return 0;

    echo "This will not be in AST";
  }

  fun(1+15*10-159);
?>
