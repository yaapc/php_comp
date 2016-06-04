<?php
class Integer{

	private static final int $MAX_VALUE = 2147483647;

  	private int $v = 0;

	public function Integer(int $value =0){
		$v = $value;
	}
		  
  public function toString() : string {
		return "Value = " + $v;
	}

	public function stringValue() : string {
		return $v+"";
	}

	public function floatValue() : float {
		return $v+0.0;
	}

	public static function compare(int $x,int $y) : int {
		if ($x == $y){
			return 0;
		}
			
		if ($x < $y){
			return $x - $y;
		}

		if ($x > $y){
			return $x - $y;
		}
	}
}
?>