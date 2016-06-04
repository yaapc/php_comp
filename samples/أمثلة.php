<?php
int $x=6;
function f(int $z,int $m):int{
	int $k=$z + $m;
	return $k;
}
int $w =5;
int $p=6;
echo f($w,$x); 
?>
////////////////
<?php
function f(int $z,int $m):int{
	int $x=4;
	int $y=5 + 6;
	int $u=$x + $y;
	return $u;
}
int $w =5;
int $p=3;
int $z=f($w,$p);
echo $z; 
?>
******************************

<?php
class a{
	int $x=5;
	int $z=8;
	function f():void{
	    int $y=5 + $x;
		return ;
	}
}
a $c=new a();
?>
******************************
<?php
	class a{
		int $x=5;
		int $z=8;
		function a(){
			$z=9;
			int $t=7;
			$t=9;
		}
	}
	a $c=new a();

?>
*************************


<?php
	class a{
		int $x=5;
		int $y=6;
		function a(){
			$x=3;			
		}
		function f():void{			
			int $z=3;
			$y= $z+ 4;
			echo $z;
			return ;
		}
	}	
	a $c=new a();
	$c->f();
	echo $c->$x;
	echo $c->$y;
?>
********************************
<?php
class a{
	int $x=4;
	function f2():void {
		int $y=3+$x;
		echo $x;
		int $zz=5+$y;
		echo $y;
		echo $zz; 
		return ;
	} 
}
a $g=new a();
$g->f2(); 
?>

*******************************
class a{
	int $y=9;
	function f():void{
	return;
	}
}
class b extends a{
	int $x=6;
	function f() : void {	
		$y=7;
		echo $y;
		return;
	} 
}
class c extends a{
	int $d=4;
	function f() : void {	
		echo $d;
		return;
	} 
}
b $x=new b();
$x->f();

************************
<?php
class a{
	static int $x=1;
	static function f(): void {
		int $y=6;
		echo $y;
		$y=1 + $x;
		echo $y;
		echo $x; 
		return;
	}
}
a $x=new a();
$x->f();

?>
*************************
<?php
class a{
	static int $x=1;
	static function f(): void {
		int $y=$x + 3;
		echo $y;
		$x=$y;
		$y=$x + 22;
		echo $y; 
		return;
	}
}
a $x=new a();
$x->f();
echo a::$x;
?>
***************************
<?php
class a{
	int $x=5;
	function f():void {
		echo $x;
		return	 ;
	}	
}
class b{
	a $x= new a();
	function f() : void {
		$x->f();
		echo 4;
		return;
	}
}
class c{
	b $y=new b();
	function f() : void {
		$y->f();
		echo 6;
		return ;
	}
}
c $r=new c();
$r->f();
?>

***************************