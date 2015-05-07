<?php 
<<<<<<< HEAD
//$pdo= new PDO('mysql:host=localhost;dbname=jeux_sequence','root','');
$pdo= new PDO('mysql::host=localhost;dbname=jeux_sequence','root','ce1mdpp');
$res=$pdo->query("select id_ins,nom_seq,sequence from sequences");
$seq=array();


      


	while($tab=$res->fetch(PDO::FETCH_NUM)){

		     array_push($seq, ">$tab[1]_$tab[0]\n $tab[2] \n\n") ; }
		     
		
	session_start();
     
	     
	foreach($seq as $element => $value)	{     
		     
		  echo $value;
                
                 
   
     }
=======
$pdo= new PDO('mysql:host=localhost;dbname=jeux_sequence','root','');
 //$pdo= new PDO('mysql::host=localhost;dbname=jeux_sequence','root','ce1mdpp');
$res=$pdo->query("select id_ins,nom_seq,sequence from sequences");
$seq=array();

	while($tab=$res->fetch(PDO::FETCH_NUM)){

		     array_push($seq, "$tab[1]_$tab[0] $tab[2]") ; }
		     
		
		     
	foreach($seq as $element => $value)	     
		     
		  echo $value;
>>>>>>> 54c0b7b4e27dd3bb89d7483f136351e70b1facbc

		
		?>
