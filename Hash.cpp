#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <iomanip>

typedef struct Entry{
    int id;
    char name[30];
};
typedef struct Hash{
    Entry entry;
};
long long ops;

//I did not use this function, only for something to convince myself
int PolynomialAccumulation(Entry entry,int sizeHashTable){
    int polynomialAccumulation=0;
    for(int i=0;i< strlen(entry.name);i++)
        polynomialAccumulation=((polynomialAccumulation*29)%sizeHashTable+entry.name[i])%sizeHashTable;

    return polynomialAccumulation;
}
int hashFunction(int key2,int key1, int i,int sizeHashTable){

    key2=0;
    return (key2+key1%sizeHashTable+(i*i*2)%sizeHashTable)%sizeHashTable;
}
void QuadraticProbing(Hash *hashTable,Entry entry,int sizeHashTable,int *pos,int type,int del){

    int cnt=0;
    int polacc= PolynomialAccumulation(entry,sizeHashTable);
    (*pos)= hashFunction(polacc,entry.id,cnt,sizeHashTable);
    //quadratic probing
    ops++;
    if(type==1) {
        if(del==0) {
            while (cnt < sizeHashTable && hashTable[(*pos)].entry.id != -1 && (hashTable[(*pos)].entry.id != entry.id || strcmp(hashTable[(*pos)].entry.name,entry.name) != 0)) {
                cnt++;
                ops++;
                (*pos) = hashFunction(polacc, entry.id, cnt, sizeHashTable);
            }
        }
        else{
            while (cnt < sizeHashTable  && (hashTable[(*pos)].entry.id != entry.id || strcmp(hashTable[(*pos)].entry.name,entry.name) != 0)) {
                cnt++;
                ops++;
                (*pos) = hashFunction(polacc, entry.id, cnt, sizeHashTable);
            }
        }
    }
    else{
        while (cnt < sizeHashTable && hashTable[(*pos)].entry.id != -1) {
            cnt++;
            ops++;
            (*pos) = hashFunction(polacc, entry.id, cnt, sizeHashTable);
        }

    }
}
void DeleteInHashTableByPos(Hash *hashTable,int sizeHashTable,int pos){
    hashTable[pos].entry.id=-1;
    hashTable[pos].entry.name[0]='\0';
}
void DeleteHashTable(Hash *hashTable,int sizeHashTable){

    for(int i=0;i<sizeHashTable;i++)
        DeleteInHashTableByPos(hashTable,sizeHashTable,i);
}
void DeleteInHashTableByEntry(Hash *hashTable,Entry entry,int sizeHashTable){

    int pos;
    QuadraticProbing(hashTable,entry,sizeHashTable,&pos,1,1);

    hashTable[pos].entry.id=-1;
    hashTable[pos].entry.name[0]='\0';
}
int InsertInHashTable(Hash *hashTable,Entry entry,int sizeHashTable,int del){

    int pos;
    QuadraticProbing(hashTable,entry,sizeHashTable,&pos,0,del);

    if(hashTable[pos].entry.id==-1){
        hashTable[pos].entry=entry;
        return 1;
    }
    return 0;

}
bool SearchInHashTable(Hash *hashTable,Entry entry,int sizeHashTable,int del){

    int pos;

    QuadraticProbing(hashTable,entry,sizeHashTable,&pos,1,del);

    ops+=1;
    if(hashTable[pos].entry.id==entry.id && strcmp(hashTable[pos].entry.name,entry.name)==0) {
      ///  printf("%d %s found at %d\n", hashTable[pos].entry.id, hashTable[pos].entry.name, pos);
        return true;
    }
    else {
   /// printf("%d %s not found in hash table\n", entry.id, entry.name);
        return false;
    }
}
void PrintHashTable(Hash *hashTable,int sizeHashTable){

    for(int i=0;i<sizeHashTable;i++){
        if(hashTable[i].entry.id==-1)
            printf("hashTable[%d]---NULL\n",i);
        else {
            printf("hashTable[%d]--- %d --- %s\n", i, hashTable[i].entry.id, hashTable[i].entry.name);
        }
    }
}
void PrintNumHashTable(Hash *hashTable,int sizeHashTable){
    int num=0;
    for(int i=0;i<sizeHashTable;i++)
        if(hashTable[i].entry.id!=-1)
            num++;

    printf("Number of elements in hash table:%d\n",num);
}
void GenerateRandomNames(int num,Hash *hashTable,int sizeHashTable){

    const char *first_names[] = {
            "Diocletian", "Tiberius", "Marcus", "Nero", "Antonius Pius",
            "Octavian", "Caligula", "Caracalla", "Hannibal", "Hadrian","Caesar",
            "Trajan", "Nerva", "Burebista", "Decebal", "Herodotus", "Plato","Aurelian",
            "Achilles", "Archimedes", "Aristotle", "Socrates","Calin","Catalin",
            "Tudor","Teodosius","Raul","Minerva","Venus","Zalmoxis","George","Razvan",
            "Cleopatra","Joan of Arc","Maria","Hilda","Kratos","Atreus","Odin","Thor",
            "Baldur","Magni","Modi","Heimdall","Loki","Thamur","Freya","Fenrir","Garmr",
            "Sigrun","Midir","Gael","Friede","William","Isshin","Genichiro","Emma","Nami",
            "Robin","Rebecca","Vivi","Perona","Bonney","Shoko","Nobara","Mai","Maki",
            "Todo","Yuta","Sukuna","Gojo","Yuiji","Kenjaku","Geto","Luffy","Zoro","Sanji",
            "Franky","Tyler","Marla","Robert","David","Francis","Alexander","Hephaestus","Atlas",
            "Atena","Ares","Heracles","Dragos","Anne-Marie","Dagon","Nicolas","Michael","Trevor",
            "Franklin","Tommy","Claude","Carl","Rick","Morty","Ash","Peter","Tony","Valerian",
            "Henry","Rykaard","Triss","Geralt","Cindy","Yennefer","Daria","Ricardo","Fredrick","Noah",
            "Novak","Raphael","Paolo","Antoine","Lionel","Cristiano","Gallileo","Isaac","Leonhard","Kurt","Taznca",
            "Vali","Leo","Travis","Koby","LeBron","Ionut","Sorin","Sergiu","Friedrich","Mengele","Joseph",
            "Lenin","Vladimir","Leona","Anubis","Ra","Toth","Osiris","Seth","Apophis","Aphelios","K'sante",
            " Jhin","Abraham", "Kuma","Mihawk","Shanks","Jimbei","Moria","Kaidou","Edward","Roger",
            "Rocks","Teach","Kenzo","Johan"
    };

    const char *last_names[] = {
            " Augustus"," Octavius"," Optimus Princeps"," of Rivia"," Odinson"," Soprano"," Vercetti",
            " Monkey D."," Aurelius"," Pop"," Popan"," Roman"," Noje"," Tresca"," Copas"," Macedonski",
            " Petrescu"," Nefertari"," Ketchum"," Darwin"," Gaius"," Commodus"," Tigoan"," Suciu"," Marinescu",
            " Alexoaie"," Merigold"," Negrean"," Ghirean"," Sas"," Ghiurutan"," Fazacas"," Siciu"," Legolas",
            " Trafalgar", " Kobain"," Chende"," Bindea"," Maxim"," Toarcas"," Flonta"," Onica"," Onicas"," Vrinceanu",
            " Moise"," Radi"," Moldovan"," Moldovanu"," Peculea"," Iancu"," Cret"," Potolea"," Cebuc"," Joldos",
            " Satorou"," Itadori"," Okkotsu"," Aoi"," Zenin"," Fushiguro"," Agamemnon"," Parker"," Johnson"," Kugisaki",
            " Sabou"," Damian"," Vele"," Man"," Butas"," Ortan"," Caprar"," Borzan"," Cornea"," Horvat"," Toma"," Groza"," Cighi",
            " Barbulescu"," Szilagy"," Gilgamesh"," Enkidou"," Alonne"," Frejlord"," Bolvar"," Arthas"," Anduin"," Sylvannus"," Emhyr",
            " Ariandel"," Kennedy"," Joldos"," Cenan"," Cena"," Holonec"," Rasa"," Gallus"," Nertan"," Lazar"," Dumuta"," Domuta",
            " Verus"," Constantin", " Kostas"," Domitian"," Guta"," Uragan"," de la Rosiori"," Vijelie"," Vinsmoke"," Roronoa"," Nico",
            " Hamilton"," Riemman"," Euler"," Gauss"," Galois"," Griezmann"," Durden"," Lagrange"," Fourier"," Cantor"," Wilhelm Leibniz",
            " Godel"," Noether"," Poincare"," Gallilei"," Pascal"," Bernoulli"," Diofant"," Brahmagupta"," Laplace"," Hilbert"," Kronecker"," Dirac",
            " Schroedinger"," Spaidar"," Bickle"," Scott"," Marx"," Zelensky"," Putin"," Gorbaciov"," Donquijote"," Saturn"," Jupiter"," Germanicus",
            " Maximus"," Basescu"," Nastase"," Tiriac"," Stolojan"," Iliescu"," Columbeanu"," Kada"," Lincoln", " Nicoara", " Balanean"," Nicoriciu",
            " Bartholomew"," Figarland"," Hakari"," Contis"," Dracula"," Gecko"," Newgate"," Gol D."," Xebec"," Marshall"," Onizuka"," Tenma"," Liebert"

    };

    int sizeFirst=162;
    int sizeLast=171;
   // printf("sizeFirst %d sizeLast %d\n",sizeFirst,sizeLast);
    srand(time(NULL));
    int cnt=0,key1,key2,id;
    Entry entry;

    while(cnt<num){

        //random id generator
        id=rand();
        key1=rand()%sizeFirst;
        key2=rand()%sizeLast;

         entry.id=id;
         entry.name[0]='\0';
        strcpy(entry.name,first_names[key1]);
        strcat(entry.name,last_names[key2]);

            cnt+=InsertInHashTable(hashTable,entry,sizeHashTable,0);
    }
   /*  if(cnt!=lastcnt)
            printf("%d\n",cnt);*/
}
void GenerateFoundUniform(Entry found[1500],Hash *hashtable,int sizeHashTable){
    srand(time(NULL));
    int cnt=0;
    Entry entry;
    while(cnt<1500) {

        entry= hashtable[rand()%sizeHashTable].entry;
        if(entry.id!=-1)
            found[cnt++]=entry;
    }
}
void GenerateNotFound(Entry notfound[1500],Hash *hashTable,int sizeHashTable){

    const char *first_names[] = {
            "Diocletian", "Tiberius", "Marcus", "Nero", "Antonius Pius",
            "Octavian", "Caligula", "Caracalla", "Hannibal", "Hadrian","Caesar",
            "Trajan", "Nerva", "Burebista", "Decebal", "Herodotus", "Plato","Aurelian",
            "Achilles", "Archimedes", "Aristotle", "Socrates","Calin","Catalin",
            "Tudor","Teodosius","Raul","Minerva","Venus","Zalmoxis","George","Razvan",
            "Cleopatra","Joan of Arc","Maria","Hilda","Kratos","Atreus","Odin","Thor",
            "Baldur","Magni","Modi","Heimdall","Loki","Thamur","Freya","Fenrir","Garmr",
            "Sigrun","Midir","Gael","Friede","William","Isshin","Genichiro","Emma","Nami",
            "Robin","Rebecca","Vivi","Perona","Bonney","Shoko","Nobara","Mai","Maki",
            "Todo","Yuta","Sukuna","Gojo","Yuiji","Kenjaku","Geto","Luffy","Zoro","Sanji",
            "Franky","Tyler","Marla","Robert","David","Francis","Alexander","Hephaestus","Atlas",
            "Atena","Ares","Heracles","Dragos","Anne-Marie","Dagon","Nicolas","Michael","Trevor",
            "Franklin","Tommy","Claude","Carl","Rick","Morty","Ash","Peter","Tony","Valerian",
            "Henry","Rykaard","Triss","Geralt","Cindy","Yennefer","Daria","Ricardo","Fredrick","Noah",
            "Novak","Raphael","Paolo","Antoine","Lionel","Cristiano","Gallileo","Isaac","Leonhard","Kurt","Taznca",
            "Vali","Leo","Travis","Koby","LeBron","Ionut","Sorin","Sergiu","Friedrich","Mengele","Joseph",
            "Lenin","Vladimir","Leona","Anubis","Ra","Toth","Osiris","Seth","Apophis","Aphelios","K'sante",
            " Jhin","Abraham", "Kuma","Mihawk","Shanks","Jimbei","Moria","Kaidou","Edward","Roger",
            "Rocks","Teach","Kenzo","Johan"
    };

    const char *last_names[] = {
            " Augustus"," Octavius"," Optimus Princeps"," of Rivia"," Odinson"," Soprano"," Vercetti",
            " Monkey D."," Aurelius"," Pop"," Popan"," Roman"," Noje"," Tresca"," Copas"," Macedonski",
            " Petrescu"," Nefertari"," Ketchum"," Darwin"," Gaius"," Commodus"," Tigoan"," Suciu"," Marinescu",
            " Alexoaie"," Merigold"," Negrean"," Ghirean"," Sas"," Ghiurutan"," Fazacas"," Siciu"," Legolas",
            " Trafalgar", " Kobain"," Chende"," Bindea"," Maxim"," Toarcas"," Flonta"," Onica"," Onicas"," Vrinceanu",
            " Moise"," Radi"," Moldovan"," Moldovanu"," Peculea"," Iancu"," Cret"," Potolea"," Cebuc"," Joldos",
            " Satorou"," Itadori"," Okkotsu"," Aoi"," Zenin"," Fushiguro"," Agamemnon"," Parker"," Johnson"," Kugisaki",
            " Sabou"," Damian"," Vele"," Man"," Butas"," Ortan"," Caprar"," Borzan"," Cornea"," Horvat"," Toma"," Groza"," Cighi",
            " Barbulescu"," Szilagy"," Gilgamesh"," Enkidou"," Alonne"," Frejlord"," Bolvar"," Arthas"," Anduin"," Sylvannus"," Emhyr",
            " Ariandel"," Kennedy"," Joldos"," Cenan"," Cena"," Holonec"," Rasa"," Gallus"," Nertan"," Lazar"," Dumuta"," Domuta",
            " Verus"," Constantin", " Kostas"," Domitian"," Guta"," Uragan"," de la Rosiori"," Vijelie"," Vinsmoke"," Roronoa"," Nico",
            " Hamilton"," Riemman"," Euler"," Gauss"," Galois"," Griezmann"," Durden"," Lagrange"," Fourier"," Cantor"," Wilhelm Leibniz",
            " Godel"," Noether"," Poincare"," Gallilei"," Pascal"," Bernoulli"," Diofant"," Brahmagupta"," Laplace"," Hilbert"," Kronecker"," Dirac",
            " Schroedinger"," Spaidar"," Bickle"," Scott"," Marx"," Zelensky"," Putin"," Gorbaciov"," Donquijote"," Saturn"," Jupiter"," Germanicus",
            " Maximus"," Basescu"," Nastase"," Tiriac"," Stolojan"," Iliescu"," Columbeanu"," Kada"," Lincoln", " Nicoara", " Balanean"," Nicoriciu",
            " Bartholomew"," Figarland"," Hakari"," Contis"," Dracula"," Gecko"," Newgate"," Gol D."," Xebec"," Marshall"," Onizuka"," Tenma"," Liebert"

    };

    int sizeFirst=162;
    int sizeLast=171;

    srand(time(NULL));
    int cnt=0,key1,key2,id;
    Entry entry;

    while(cnt<1500){

        //random id generator
        id=rand();
        key1=rand()%sizeFirst;
        key2=rand()%sizeLast;

        entry.id=id;
        entry.name[0]='\0';
        strcpy(entry.name,first_names[key1]);
        strcat(entry.name,last_names[key2]);

        if(false==SearchInHashTable(hashTable,entry,sizeHashTable,0))
            notfound[cnt++]=entry;
    }

}
void DemoSearchInsert(Hash *hashTable,int sizeHashTable){

    GenerateRandomNames(10000,hashTable,sizeHashTable);
    PrintHashTable(hashTable,sizeHashTable);
    //PrintNumHashTable(hashTable,sizeHashTable);

   // long long avgops,maxops=-1;
    Entry searchEntry;
    for(int i=0;i<sizeHashTable;i++)
        if(hashTable[i].entry.id!=-1){
            ops=0;
            searchEntry.id= hashTable[i].entry.id;
            searchEntry.name[0]='\0';
            strcpy(searchEntry.name,hashTable[i].entry.name);
         //   printf("%d %d-%s %d-%s --%d\n",i,hashTable[i].entry.id,hashTable[i].entry.name,searchEntry.id,searchEntry.name,
          //         strcmp(searchEntry.name,hashTable[i].entry.name));
            SearchInHashTable(hashTable,searchEntry,sizeHashTable,0);

           // printf("%d \n",ops);
        }

   // PrintNumHashTable(hashTable,sizeHashTable);
  //  printf("Average ops:%d\n Max ops:%d\n",avgops,maxops);
}
void Fill95NonUniform(Hash *hashTable,int sizeHashTable){
    long long maxops = -1;
    unsigned long long Totalops=0;
   // PrintHashTable(hashTable,sizeHashTable);

        Entry searchEntry;
        for(int test=0;test<2000;test++) {
            GenerateRandomNames(9566, hashTable, sizeHashTable);
         //   PrintNumHashTable(hashTable,sizeHashTable);
            for (int i = 0; i < 3000; i++)
                if (hashTable[i].entry.id != -1) {
                    ops = 0;
                    searchEntry.id = hashTable[i].entry.id;
                    searchEntry.name[0] = '\0';
                    strcpy(searchEntry.name, hashTable[i].entry.name);
                    SearchInHashTable(hashTable, searchEntry, sizeHashTable,0);
                    Totalops += ops;
                    maxops = (maxops > ops) ? maxops : ops;
                    // printf("%d \n",ops);
                }
            DeleteHashTable(hashTable, sizeHashTable);
        }

        printf("for fill factor %d% with non-uniform selection of the found elements: Avg effort %lf | Max effort %lld\n",95,(double)Totalops/3000/2000,maxops);

}

void UniformSelection(Hash *hashTable,int sizeHashTable){

     int fill[5]={80,85,90,95,99};
    // for fill factors 80%  85%  90%  95%  99%
          int sizes[5]={8055,8559,9062,9566,9968}; // number of inserted elements
    long long maxopsfound[5]={-1,-1,-1,-1,-1};
    unsigned long long Totalopsfound[5]={0,0,0,0,0};
    long long maxopsnotfound[5]={-1,-1,-1,-1,-1};
    unsigned long long Totalopsnotfound[5]={0,0,0,0,0};
    Entry found[1500],notfound[1500];
  for(int i=0;i<5;i++){
     GenerateRandomNames(sizes[i], hashTable, sizeHashTable);
     GenerateFoundUniform(found,hashTable,sizeHashTable);
     GenerateNotFound(notfound,hashTable,sizeHashTable);
     for (int j=0;j<1500;j++)
     {
         ops=0;
        if(false==SearchInHashTable(hashTable,found[j],sizeHashTable,0) || found[j].id==-1)
             printf("Hopa\n");
         Totalopsfound[i]+=ops;
         maxopsfound[i]=(maxopsfound[i]>ops) ? maxopsfound[i]:ops;
     }

     for (int j=0;j<1500;j++)
     {
         ops=0;
         if(true==SearchInHashTable(hashTable,notfound[j],sizeHashTable,0) || notfound[j].id==-1)
             printf("Hopa\n");
         Totalopsnotfound[i]+=ops;
         maxopsnotfound[i]=(maxopsnotfound[i]>ops) ? maxopsnotfound[i]:ops;
     }
      DeleteHashTable(hashTable, sizeHashTable);

      printf("for fill factor %d%: | Avg effort found %lf | Max effort found %d | Avg effort not found %lf | Max effort not found %d|\n",fill[i],(double)Totalopsfound[i]/1500,maxopsfound[i],(double)Totalopsnotfound[i]/1500,maxopsnotfound[i]);
  }

}
void SearchAfterDelete(Hash *hashTable,int sizeHashTable){

    GenerateRandomNames(9968, hashTable, sizeHashTable);
  //  PrintNumHashTable(hashTable,sizeHashTable);
    Entry found[1500],notfound[1500];
    int cnt=0,pos;
    srand(time(NULL));
    while(cnt<1913){
        pos=rand()*7%sizeHashTable;
        if(hashTable[pos].entry.id!=-1){
            if(cnt<1500)
            notfound[cnt]=hashTable[pos].entry;

            DeleteInHashTableByPos(hashTable,sizeHashTable,pos);
            cnt++;
        }
    }
  //  PrintNumHashTable(hashTable,sizeHashTable);
    GenerateFoundUniform(found,hashTable,sizeHashTable);

    long long maxeffortfound=-1,maxeffortnotfound=-1,Totalopsfound=0,Totalopsnotfound=0;

    for(int i=0;i<1500;i++){
        ops=0;
        SearchInHashTable(hashTable,found[i],sizeHashTable,1);
        Totalopsfound+=ops;
        maxeffortfound=(maxeffortfound>ops) ? maxeffortfound:ops;
    }

    for(int i=0;i<1500;i++){
        ops=0;
        SearchInHashTable(hashTable,notfound[i],sizeHashTable,1);
        Totalopsnotfound+=ops;
        maxeffortnotfound=(maxeffortnotfound>ops) ? maxeffortnotfound:ops;
    }
    printf("for fill factor %d% -> %d% | after deletions: Avg effort found %lf | Max effort found %d | Avg effort not found %lf | Max effort not found %d\n",99,80,(double)Totalopsfound/1500,maxeffortfound,(double)Totalopsnotfound/1500,maxeffortnotfound);
}
int main(){
   /*
    bool sieve[1000001];
    freopen("output.out","w",stdout);
    for(int i=2;i*i<1000000;i++)
        if(!sieve[i]){
            for(int j=i*i;j<=1000000;j+=i)
                sieve[j]=true;
        }

    for(int i=2;i<=1000000;i++)
        if(!sieve[i])
            printf("(%d)",i);*/

   freopen("output.out","w",stdout);
   int sizeHashTable=10069;
   Hash *hashTable=(Hash *) malloc(sizeof(Hash)*sizeHashTable);
   //consider empty cells as having the id=-1
   for(int i=0;i<sizeHashTable;i++){
          hashTable[i].entry.id=-1;
    }


   // PrintNumHashTable(hashTable,sizeHashTable);
    //PrintHashTable(hashTable,sizeHashTable);
    DemoSearchInsert(hashTable,sizeHashTable);
//    printf("---non-uniform selection of the elements with  loading factor95%---\n");
//   Fill95NonUniform(hashTable,sizeHashTable);
//    printf("---Uniform selection of the found elements with different loading factors---\n");
//  UniformSelection(hashTable,sizeHashTable);
//    printf("---Search After delete with uniform selection of the elements---\n");
//   SearchAfterDelete(hashTable,sizeHashTable);
//    free(hashTable);
     return 0;
}