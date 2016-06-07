<html>
<body>
<?php

class Vehicle{

	 function  Vehicle(){}

		function move() : void {
			echo "Vehicles can move !!";
		}
}

class MotorBike extends Vehicle{

		function  MotorBike(){}

		function move() : void {
			echo "MotorBike can move !!";
		}

		function toString() : string {
			return "MotorBike";
		}
}

class Track extends Vehicle{

		function  Track(){}

		function move() : void {
			echo "Track can move !!";
	 }

		 function toString() : string {
			return "Track";
		}
 }

 class Bicycle extends Vehicle{

		function  Bicycle(){}
		function move() : void {
			echo "Bicycle can move !!";
	 }

		 function toString() : string {
			return "Bicycle";
		}
}

   int $numOfVehicles = 2;
   Vehicle $myVeh;


    echo "<center>";
    echo "<h2>Counting Vehicles:</h2>";
		echo "<div style='color: red' >";
		for(int $i = 0 ; $i < $numOfVehicles ; $i += 1){
					if ($i == 0){
             $myVeh = new MotorBike();
					}else {
						 $myVeh = new Track();
					}
					string $vehString = $myVeh->toString();
					echo "<h3>$vehString</h3>";
		}
		echo "</div>";

		string $url = "http://helloworld.com";

 ?>

<a href='<?php echo $url; ?>'>Click to Buy!</a>

<?php   echo "</center>"; ?>
</body>
</html>
