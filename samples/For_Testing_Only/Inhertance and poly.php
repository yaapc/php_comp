<?php
	class Parent
	{
		  private static int $staticX = 50;
		
		  public int $x;
		  public int $y;

		  function Parent()
      {
        $staticX += 100;
        $x = $staticX;
      }


		  function fun() : void {
		    echo "I am Parent Method";
		  }

		  function fun2() : void {
		    echo "I am also Parent Method";
		  }
	}

	class Child extends  Parent
	{
		  function Child(){}
		  function fun() : void {
		    echo "I am Child Method";
		  }
	}




    Parent $p;
  
    if (true)
    {
      $p = new Child();
    }else
    {
      $p = new Parent();
    }

    $p->fun();
    $p->fun2();
  
  
  echo Parent::$staticX;

?>