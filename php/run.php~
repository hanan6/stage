<?php
		
    $postdata = file_get_contents("php://input");
    $request = json_decode($postdata);
    @$texte = $request->texte;
   
    $monfichier= fopen("../MDP/instances/LEAP/leap_1.fasta",'w');
    fwrite($monfichier,$texte); 

    system("../MDP/./build/MDP2 -i ../MDP/instances/LEAP/leap_1.fasta -o ../MDP/results/leap_1.xml -D 99 2>&1");

// ---------------------- resultats ------------------------------------------------------------------------



$pdo= new PDO('mysql::host=localhost;dbname=resultat','root','ce1mdpp');
$requete_bloc = $pdo->prepare("insert into bloc values(?,?,?,?);");
$requete_motif = $pdo->prepare("insert into motif values(?,?,?,?);");
$requete_solution = $pdo->prepare("insert into solution values(?,?,?,?,?);");
$requete_date=$pdo->query("select NOW();");
$requete_result=$pdo->query("SELECT id_result FROM motif ORDER BY id_result DESC LIMIT 1 ;");

$res=$requete_result->fetch(PDO::FETCH_NUM);
$date=$requete_date->fetch(PDO::FETCH_NUM);

$xml=simplexml_load_file("../MDP/results/leap_1.xml");

 $nom_ins="LEAP";
 $i=0;
 $id_bloc=0;
 $res[0]=$res[0]+1; 
 

foreach($xml->class as $classe)

{
         
         if ($classe->satisfied["val"]=="true"){
           
             $motif="M" . $i=$i+1;
             $id_ins=$classe["name"];
             
             foreach($classe->motif->patterns->pattern as $patron){
             $id_bloc=$id_bloc+1;
             $blo=$requete_bloc->execute(Array($res[0],$id_bloc,$patron,$date[0]));
             $mot=$requete_motif->execute(Array($res[0],$motif,$id_bloc,$date[0]));
             
             };
             $sol=$requete_solution->execute(Array($res[0],$nom_ins,$id_ins,$motif,$date[0]));
                
           };
	
};

//-------------------------------------------------------------------------------------------------
		
?>
