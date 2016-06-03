<?php
	function sum(int $x, int $y): int {
		return $x + $y;
	}

	function printSum(int $x, int $y) : void
	{
		int $rd= sum($x,$y);
		echo "$x + $y = $rd";
	}

	printSum(10,10);
?>
----------------------------------------------------------
10 + 10 = 20