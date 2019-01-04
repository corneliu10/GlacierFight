# GlacierFight

Link: https://ocw.cs.pub.ro/courses/programare/teme_2018/tema3_2018_ca?fbclid=IwAR3PSMebtWmz10yhFjNQHeuocTGi_F9oLUKHnbMYwRMt8ol9wtqEjj0Kz7Y

### Am folosit ca structuri CelulaGhetar (dmg, h, idulSpiridus (-1 daca nu se afla nimeni in celula)) si Spiridus (hp, stamina, ...) pentru a reprezenta datele intr-un format favorabil: 
CelulaGhetar ghetar[NMAX][NMAX];
Spiridus spiridusi[NMAX];
 
### Pentru snowstorm am folosit un union pentru a sparge int32 in 4 bytes:
union {
    unsigned long storm;
    unsigned char bytes[4];
} Storm;

### Pentru scoreboard am folosit qsort cu un comparator pe structura Spiridus.

### Dupa ce citesc datele despre ghetar si spiridusi citesc pe rand fiecare linie si aplic comanda respectiva liniei:
  MOVE - mut jucatorul pe ghetar, in cazul in care se intalneste cu alt jucator ii pun sa se bata, iar daca el castiga, continui miscare, daca nu, comanda s-a terminat
  MELTDOWN - micsorez raza ghetarului si verific fiecare spiridus daca a intrat la apa
  SNOWSTORM - extrag cei 4 bytes si verific daca a fost afectat vreun spiridus
  SCORE_BOARD - copiez vectorul cu spiridusi si sortez copia cu filtrul comparator
  
### La final verific daca a mai ramas doar un singur player pe ghetar, caz in care il afisez
