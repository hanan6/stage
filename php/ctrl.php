<?php 
$pdo= new PDO('mysql:host=localhost;dbname=jeux_sequence','root','');
$res=$pdo->query("select id_ins,nom_seq,sequence from sequences");
$seq=array();

	while($tab=$res->fetch(PDO::FETCH_NUM)){

		     array_push($seq, "$tab[1]_$tab[0] $tab[2]") ; }
		     
		
		     
	foreach($seq as $element => $value)	     
		     
		  echo $value;

		
		?>
