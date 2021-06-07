#include<iostream>
#include<ctime>
#include <cstring>
#include <windows.h> 
#define GORNJA_GRANICA 0
#define DONJA_GRANICA 5
#define LIJEVA_GRANICA 0
#define DESNA_GRANICA 5
#define DULJINA_CIGLE 8
using namespace std;

//IZVORI
//za znakove npr. 254: http://www.asciitable.com/
//objasnjenje boja: https://mycodecollection.blogspot.com/2014/12/console-fun-colors.html

void nacrtaj(int x, int y, int odabrano);
bool igraj(void);
void postavi_boju(int kod_boje);
bool dopustena_zamjena(int x_o, int y_o, int x_t, int y_t);
bool match(void);
bool pobjeda(void);
void ispis_uputa(void);

HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
//ovi brojevi su kodovi za boje kako su one definirane u windows.h, enum koristim radi preglednosti koja boja je koja
enum Boja {CRNA = 0, TAMNOPLAVA = 1, ZELENA = 2, SMEDJA = 4, LJUBICASTA = 5, BIJELOSIVA = 7, PLAVA = 9, LIME = 10, CRVENA = 12, ZUTA = 14, BIJELA = 15};
int boje[7] = {TAMNOPLAVA, ZELENA, SMEDJA, LJUBICASTA, PLAVA, CRVENA, ZUTA}; //bijelosiva samo za trenutnu ciglu, bijela za oznacenu, crna za unistene

int cigle[DONJA_GRANICA][DESNA_GRANICA];
bool tablica_postoji;
bool win = false;
int x = 0;
int y = 0; //x i y pozicije trenutne cigle, definirane ovdje umjesto kao static int u igraj jer ako idemo na novu igru moramo resetirati na nule sto ne mozemo sa static int u igraj

string upute1 = "Cilj igre je zamjenjivati cigle tako da dobijete tri jednake boje vodoravno ili okomito" 
	" i te cigle se uklanjaju. Pobjedjujete kada ne preostane vise nijedna kombinacija cigli koju je moguce ukloniti."
	"\n\nKoristite ";
string upute2 = "wasd ";
string upute3 = "tipke za kretanje. (w+Enter - jedno mjesto prema gore, d+Enter - jednom udesno itd.) Nalazite se"
	" na onoj cigli koja je oznacena bijelosivo. Za kretanje mozete i unositi vise od jednog odabira odjednom, npr. "
	"umjesto unosenja jednog po jednog odabira mozete unijeti dddss+Enter i pomaknut cete se za tri mjesta udesno, dva "
	"mjesta dolje.\n\nMozete odabrati dvije cigle za zamjenu njihovih mjesta. Za odabir cigle na kojoj se trenutacno nalazite "
	"unesite slovo ";
string upute4 = "o";
string upute5 = ". Odabrana cigla poprimit ce bijelu boju. Druga cigla koju odaberete za zamjenu moze biti jedno mjesto "
	"ispod, iznad, lijevo ili desno od prve odabrane cigle. (Dalje od toga zamjena nece biti dozvoljena.) ";
string upute6 = "\n\nMoguce je kombinirati unose radi brzeg igranja, npr. owo+Enter za zamjenu gornje i donje cigle, "
	"owoowo\nza prijenos cigle odozdo prema gore za dva mjesta, odo za zamjenu lijeve cigle nadesno,\nodoodoodo za tri zamjene "
	"prema desno, owoodo za prijenos cigle prema gore i zatim prema desno,\ni analogno za ostale smjerove "
	"(broj ponavljanja kombinacija neogranicen osim fizickim granicama tablice).";
string upute7 = "\n\nAko zelite prekinuti igru, unesite ";
string upute8 = "q";
string upute9 =	".\n-----------------------------------------------------------------\n";

int main(void) 
{
	bool nastavi_igrati;
	char nova_igra;
	srand(time(NULL));
	
	do {
		do {//crtaj prvu tablicu sve dok ne dobijemo tablicu u kojoj nema zamjena
			system("cls");
			tablica_postoji = false;
			nacrtaj(0, 0, false);
		} while (match());
		//za nacrtaj() gore stavljam 0,0 jer postavljamo trenutnu ciglu na najgornju lijevu (prvu u polju), false jer korisnik jos nije odabrao nijednu ciglu
		while(true) {
			nastavi_igrati = igraj();
			if (!nastavi_igrati) break;
			if (win) 
			{
				(rand()%2 == 0) ? postavi_boju(CRVENA) : ((rand()%2 == 0) ? postavi_boju(PLAVA) : postavi_boju(ZELENA));
				cout << endl << "Pobijedili ste!\n";
				break;
			}
		}
		postavi_boju(ZELENA);
		cout << "\nNova igra? d/n   "; cin >> nova_igra;
		win = false;
		x = 0;
		y = 0;
	
	} while (nova_igra == 'd' || nova_igra == 'D');
	
	postavi_boju(BIJELA); //vrati na default bijelu
    return 0;
} 

void nacrtaj(int x_trenutno, int y_trenutno, int odabrano)
{	
	static int x_odabrano = -1;
	static int y_odabrano = -1;
	static int x_odabrano2 = -1;
	static int y_odabrano2 = -1;
		
	if (tablica_postoji) system("cls");
	ispis_uputa();
	for (int i = 0; i < DONJA_GRANICA; i++) //redovi
	{
		cout << endl << "                     "; //prazni prostor na poèetku svakog reda
		for (int j = 0; j < DESNA_GRANICA; j++) //stupci
		{
			if (!tablica_postoji) cigle[i][j] = boje[rand()%7]; //dajemo svakoj cigli njenu boju, samo jednom na pocetku
			
			if (odabrano && x_odabrano != -1 && dopustena_zamjena(x_odabrano, y_odabrano, x_trenutno, y_trenutno)) {
				//onda znam da je drugi odabir napravljen
				x_odabrano2 = x_trenutno;
				y_odabrano2 = y_trenutno;
			}//treba mi jos ovaj odsjecak koda plus ovaj dolje za taj drugi odabir zbog specificnosti crtanja
			//lijevo prema desno i gore prema dolje, jer bez ovog odsjecka koda tu ne bi imali zeljeno ponasanje
			//kod zamjene gdje prva odabrana cigla vise desno tj vise dolje od druge (informacija ne bi na vrijeme dosla do
			//druge)
			if (i == x_trenutno && j == y_trenutno) {
				if (odabrano) {
					//znaci da je ovo prva odabrana cigla
					if (x_odabrano == -1 && y_odabrano == -1) {
						postavi_boju(BIJELA); 
						x_odabrano = i; 
						y_odabrano = j;
					} 
					//znaci da je druga odabrana cigla ako su xy_odabrano != -1, samo jos ubacimo uvjer provjere
					//je li druga odabrana cigla pokraj prve odabrane (dalje od toga nije dozvoljeno odabrati drugu)
					else if (dopustena_zamjena(x_odabrano, y_odabrano, x_trenutno, y_trenutno)) {
						x_odabrano2 = x_trenutno;
						y_odabrano2 = y_trenutno;
						postavi_boju(BIJELOSIVA);

						int pom = cigle[x_odabrano][y_odabrano];
						cigle[x_odabrano][y_odabrano] = cigle[x_odabrano2][y_odabrano2];
						cigle[x_odabrano2][y_odabrano2] = pom;
						
						x_odabrano = -1;
						y_odabrano = -1;
						x_odabrano2 = -1;
						y_odabrano2 = -1;
																		
					} 
					else postavi_boju(BIJELOSIVA); //zamjena nije dopuštena, pa samo nastavi crtati kursor na tom trenutnom mjestu
					
				} //kod gore za odabrano izvrsit ce se samo u 1 passu jer nakon povratka iz fje igraj() uvijek stavi odabrano na false, pa zato ovdje pamtimo poziciju odabranog
				else postavi_boju(BIJELOSIVA);
			}
			else if (i == x_odabrano && j == y_odabrano)
			{
				if (!odabrano) postavi_boju(BIJELA); //nismo jos odabrali drugu
				else
				{
					if (x_odabrano2 == -1 && y_odabrano2 == -1) postavi_boju(BIJELA); //taj "odabir" nije trazeni drugi odabir koji nas zanima
					else postavi_boju(cigle[x_odabrano2][y_odabrano2]); //drugi odabir
				}
			}
			else postavi_boju(cigle[i][j]);	
			
			for (int k = 0; k < DULJINA_CIGLE; k++) cout << char(219); //tu actually crtamo jednu ciglu - 219 je ASCII za mali pravokutnik
			
		}
	}
	if (!tablica_postoji) tablica_postoji = true;
	cout << endl;
	return;
}

bool igraj(void)
{
	//je li trenutno odabrano
	bool odabrano = false;
	
	char odabir;
	postavi_boju(BIJELA); //radi upisa vrati na default bijelu	
	cin >> odabir;
	//kretanje
	if (odabir == 'w' && x-1 >= GORNJA_GRANICA) x--;
	else if (odabir == 's' && x+1 < DONJA_GRANICA) x++;
	else if (odabir == 'a' && y-1 >= LIJEVA_GRANICA) y--;
	else if(odabir == 'd' && y+1 < DESNA_GRANICA) y++;
	//odabir trenutne cigle
	else if (odabir == 'o') odabrano = true;
	else if (odabir == 'q') return false;
	
	match(); //napravimo match PRIJE svakog crtanja kako bi se prije cekiranja pobjede stiglo nacrtati crne cigle
	nacrtaj(x, y, odabrano);
	if (pobjeda()) win = true;
	
	return true;
}

void postavi_boju(int kod_boje) 
{  
    SetConsoleTextAttribute(out, kod_boje);  
}

bool dopustena_zamjena(int x_o, int y_o, int x_t, int y_t)
{
	if (x_o == x_t - 1 && y_o == y_t || x_o == x_t + 1 && y_o == y_t ||
		y_o == y_t - 1 && x_o == x_t || y_o == y_t + 1 && x_o == x_t)
			return true;
	return false;
}

bool match(void)
{
	bool zamjena = false;
	
	for (int i = 0; i < DONJA_GRANICA - 2; i++)
		for (int j = 0; j < DESNA_GRANICA; j++)
			if ((cigle[i][j] == cigle[i+1][j] && cigle[i][j] == cigle[i+2][j])) 
			{
				cigle[i][j] = cigle[i+1][j] = cigle[i+2][j] = CRNA;
				zamjena = true;
			}
			
	for (int i = 0; i < DONJA_GRANICA; i++)
		for (int j = 0; j < DESNA_GRANICA - 2; j++)	
			if ((cigle[i][j] == cigle[i][j+1] && cigle[i][j] == cigle[i][j+2])) 
			{
				cigle[i][j] = cigle[i][j+1] = cigle[i][j+2] = CRNA;
				zamjena = true;
			}
	
	return zamjena;
}

bool pobjeda(void)
{
	int brojac_boja[15] = {0}; //15 jer je max vrijednost ZUTA s 14
	
	for (int i = 0; i < DONJA_GRANICA; i++)
		for (int j = 0; j < DESNA_GRANICA; j++)
			brojac_boja[cigle[i][j]]++;
			
	for (int i = 1; i < 15; i++) if (brojac_boja[i] >= 3) return false; //Vazno: int i = 1 jer moramo izuzeti crnu iz razmatranja!!!
	
	return true;
}

void ispis_uputa(void)
{
	postavi_boju(LIME);
	cout << "UPUTE: ";
	postavi_boju(BIJELA);
	cout << upute1;
	postavi_boju(ZUTA);
	cout << upute2;
	postavi_boju(BIJELA);
	cout << upute3;
	postavi_boju(ZUTA);
	cout << upute4;
	postavi_boju(BIJELA);
	cout << upute5;
	postavi_boju(ZUTA);
	cout << upute6;
	postavi_boju(BIJELA);
	cout << upute7;
	postavi_boju(ZUTA);
	cout << upute8;
	postavi_boju(BIJELA);
	cout << upute9;	
	return;
}
