<?php


	class DB_Manager{
		
		
		private static DB_Manager $instance;

		public static function getInstance() : DB_Manager{
			DB_Manager $null;
			if($instance == $null){
				$instance = new DB_Manager();
				return $instance;
			}else {
				return $instance;
			}
		}
		
		private function DB_Manager(){
		}

		public string $IP_ADDRESS = "127.0.0.1";
		public string $PORT = "8888";

	}
	

	DB_Manager $dbManager = DB_Manager::getInstance();
	echo $dbManager->IP_ADDRESS;

	$dbManager->IP_ADDRESS = "192.168.1.1";

	DB_Manager $dbManager2 = DB_Manager::getInstance();
	echo $dbManager2->IP_ADDRESS;

?>