INSTALLATION

1- installer Gecode [dans le $HOME]
cd
mkdir Gecode
cd Gecode
wget http://www.gecode.org/download/gecode-4.2.1.tar.gz
tar -xf gecode-4.2.1.tar.gz
cd gecode-4.2.1.tar.gz
./configure
make install
cd
echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/Logiciels/gecode-4.2.1" >> .bashrc
echo "export LD_LIBRARY_PATH" >> .bashrc

- installer mdp [dans le répertoire racine du dépôt git]
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make




EXECUTION

[1] ./build/MDP -i instances/leap_1423128908.fasta -o results/leap_1423128908.xml -D 99
Exécution classique pour résoudre le rmpd
-i nom du fichier en entrée au foramt FASTA
-o nom du fichier en sortie
-D 99 indique que la classe 99 du fichier FASTA ne doit pas être prise en compte.
Plusieurs classes peuvent être passées en paramètre -D 99:100

[2] ./build/MDP -i instances/leap_1423128908.fasta -C results/leap_1423128908.xml -D 99
Convertit le fichier FASTA en entrée qui est compatible avec verif.d

[3] ./build/MDP -i instances/leap_1423128908.fasta -s results/leap_pat -D 99
Analyse détaillée de l'évolution des patrons lors de la première phase. Plusieurs fichiers vont être créés, ils seront situés dans le répertoire results et commenceront tous par le préfixe leap_pat (les fichiers sont au format csv avec la tabulation comme séparateur).

[3] ./build/MDP -i instances/leap_1423128908.fasta -t results/leap_time -D 99
Analyse détaillée du temps d'exécution de la première phase en fonction du slack. Plusieurs fichiers vont être créés, ils seront situés dans le répertoire results et commenceront tous par le préfixe leap_time (les fichiers sont au format csv avec la tabulation comme séparateur).

[4] ./build/MDP -i instances/leap_1423128908.fasta -p results/leap_pat -D 99
Analyse détaillée du nombre de patrons de la première phase en fonction du slack. Plusieurs fichiers vont être créés, ils seront situés dans le répertoire results et commenceront tous par le préfixe leap_pat (les fichiers sont au format csv avec la tabulation comme séparateur).

[5] ./build/MDP -i instances/leap_1423128908.fasta -g results/leap_emb -D 99
Génère des fichiers qui seront utilisés par la suite pour la recherche d'ordres partiels.


-m, -M VALUE peuvent être utilisés pour respectivement fixé le slack maximum et le slack minimum.

-n VALUE peut être utilisé pour fixer le nombre de threads à utiliser
