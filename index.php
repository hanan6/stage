<?php 
$pdo= new PDO('mysql::host=localhost;dbname=jeux_sequence','root','ce1mdpp');
$res=$pdo->query("select id_ins,nom_seq,sequence from sequences");
$seq=array();

	while($tab=$res->fetch(PDO::FETCH_NUM)){

		     array_push($seq, ">$tab[1]_$tab[0] </br> $tab[2] </br>") ; }
		     
		
		     
	//foreach($seq as $element => $value)	     
		     
		//  echo $value;
		   
		
	
		//fopen("MDP/instances/LEAP/leap_1.fasta","w");
		//fwrite("MDP/instances/LEAP/leap_1.fasta",$_POST['textarea']);
		//system("MDP/./build/MDP2 -i MDP/instances/LEAP/leap_1.fasta -o MDP/results/leap_1.xml -D 99 2>&1");
	

		
		?>
