<?php
    int $i = 100;
    echo $i;
	    for(int $i = 0 ; $i < 1000; $i = $i + 1)
		  {
		     if ($i == 20)
			    break;
		   
		    if ($i % 2 == 0)
			  continue;
		
		    echo "I = $i";
		  }
    echo $i;
?>