<?php
	function funcFloat(float $a = 1.0, float $b = 2.0,float $c = 3.0) : void {
		echo $a,"\t",$b,"\t",$c;
		echo "\n";
	}
		  
	funcFloat();

	funcFloat(54.0);
	  
	funcFloat(10.0,145.0);
	 
	funcFloat(3.0,2.0,1.0);
?>