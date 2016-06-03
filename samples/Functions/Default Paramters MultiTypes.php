<?php
	function funcMultiTypes(int $a = 1, float $b = 2.0,string $c = "3") : void {
		echo $a,"\t",$b,"\t",$c;
		echo "\n";
	}
		  
	funcMultiTypes();

	funcMultiTypes(54);
	  
	funcMultiTypes(10,15.0);
	 
	funcMultiTypes(0,400.15,"HaHa");
?>