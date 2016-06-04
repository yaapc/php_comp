<?php
	function funcString(string $a = "A", string $b = "B",string $c = "C") : void {
		echo $a,"\t",$b,"\t",$c;
		echo "\n";
	}
		  
	funcString();

	funcString("D");
	  
	funcString("X","Y");
	 
	funcString("c","b","a");
?>