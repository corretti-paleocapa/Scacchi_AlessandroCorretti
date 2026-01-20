#include <iostream>
#include <string>
#include <cmath>

using namespace std;

int turnoGiocatore = 1;

//Variabili per salavare le posizioni per arrocco corto
bool reBiancoMosso = false;
bool reNeroMosso = false;
bool torreA1Mossa = false;
bool torreH1Mossa = false;
bool torreA8Mossa = false;
bool torreH8Mossa = false;
//Variabili per salavare le posizioni per en passant
int pedoneDoppioPassoRiga = -1;
int pedoneDoppioPassoCol = -1;

string scacchiera[8][8] = {
    {"♜","♞","♝","♛","♚","♝","♞","♜"},
    {"♟","♟","♟","♟","♟","♟","♟","♟"},
    {" "," "," "," "," "," "," "," "},
    {" "," "," "," "," "," "," "," "},
    {" "," "," "," "," "," "," "," "},
    {" "," "," "," "," "," "," "," "},
    {"♙","♙","♙","♙","♙","♙","♙","♙"},
    {"♖","♘","♗","♕","♔","♗","♘","♖"}
};

//Prototipi funzioni per la funzione sotto ->'reSottoAttacco'
bool mossaValida(int colPartenza, int rigaPartenza, int colArrivo, int rigaArrivo);
bool isPezzoMio(int colonna, int riga);

void pulisciSchermo() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

bool ilReEsiste(string tipoRe) {
    for (int riga = 0; riga < 8; riga++) {
        for (int colonna = 0; colonna < 8; colonna++) {
            if (scacchiera[riga][colonna] == tipoRe) return true;
        }
    }
    return false;
}

bool reSottoAttacco() {
    string mioRe;
    if (turnoGiocatore % 2 != 0) mioRe = "♔";
    else mioRe = "♚";

    int rigaRe = -1;
    int colonnaRe = -1;

    for (int riga = 0; riga < 8; riga++) {
        for (int colonna = 0; colonna < 8; colonna++) {
            if (scacchiera[riga][colonna] == mioRe) {
                rigaRe = riga;
                colonnaRe = colonna;
            }
        }
    }

    if (rigaRe == -1) return false;
    //simulazione turno avversario
    turnoGiocatore++;

    for (int riga = 0; riga < 8; riga++) {
        for (int colonna = 0; colonna < 8; colonna++) {
            if (isPezzoMio(colonna, riga)) {
                if (mossaValida(colonna, riga, colonnaRe, rigaRe)) {
                    turnoGiocatore--;//fine simulazione con re sotto scacco
                    return true;
                }
            }
        }
    }

    turnoGiocatore--;//fine simulazione senza re sotto scacco
    return false;
}

void stampaScacchiera() {
    for (int riga = 0; riga < 8; riga++) {
        cout << 8 - riga << "|";
        for (int colonna = 0; colonna < 8; colonna++) {
            if ((riga + colonna) % 2 == 0) cout << "\033[48;5;180m";
            else cout << "\033[48;5;94m";
            cout << " " << scacchiera[riga][colonna] << "  ";
        }
        cout << "\033[0m\n";
    }
    cout << "   a   b   c   d   e   f   g   h\n";
}

void promozionePedone(int colonna, int riga) {
    string pezzo = scacchiera[riga][colonna];
    bool biancoArrivato = (pezzo == "♙" && riga == 0);
    bool neroArrivato = (pezzo == "♟" && riga == 7);

    if (biancoArrivato || neroArrivato) {
        cout << "PROMOZIONE DEL PEDONE! Scegli un pezzo (D=Regina, T=Torre, A=Alfiere, C=Cavallo): ";
        char scelta;
        cin >> scelta;

        if (biancoArrivato) {
            if (scelta == 'T' || scelta == 't') scacchiera[riga][colonna] = "♖";
            else if (scelta == 'A' || scelta == 'a') scacchiera[riga][colonna] = "♗";
            else if (scelta == 'C' || scelta == 'c') scacchiera[riga][colonna] = "♘";
            else scacchiera[riga][colonna] = "♕";
        } else {
            if (scelta == 'T' || scelta == 't') scacchiera[riga][colonna] = "♜";
            else if (scelta == 'A' || scelta == 'a') scacchiera[riga][colonna] = "♝";
            else if (scelta == 'C' || scelta == 'c') scacchiera[riga][colonna] = "♞";
            else scacchiera[riga][colonna] = "♛";
        }
    }
}

bool isPezzoMio(int colonna, int riga) {
    string pezzo = scacchiera[riga][colonna];
    if (pezzo == " ") return false;

    string pezziBianchi = "♖♘♗♕♔♙";
    string pezziNeri = "♜♞♝♛♚♟";

    if (turnoGiocatore % 2 != 0 && pezziBianchi.find(pezzo) != string::npos) return true;
    if (turnoGiocatore % 2 == 0 && pezziNeri.find(pezzo) != string::npos) return true;

    return false;
}

bool stradaLibera(int colInizio, int rigaInizio, int diffCol, int diffRiga) {
    int direzioneColonna = 0;
    int direzioneRiga = 0;

    if (diffCol > 0) direzioneColonna = 1;
    if (diffCol < 0) direzioneColonna = -1;
    if (diffRiga > 0) direzioneRiga = 1;
    if (diffRiga < 0) direzioneRiga = -1;

    int colCorrente = colInizio + direzioneColonna;
    int rigaCorrente = rigaInizio + direzioneRiga;

    while (colCorrente != colInizio + diffCol || rigaCorrente != rigaInizio + diffRiga) {
        if (scacchiera[rigaCorrente][colCorrente] != " ") return false;
        colCorrente += direzioneColonna;
        rigaCorrente += direzioneRiga;
    }
    return true;
}

bool mossaValida(int colPartenza, int rigaPartenza, int colArrivo, int rigaArrivo) {
    string pezzo = scacchiera[rigaPartenza][colPartenza];
    int diffCol = colArrivo - colPartenza;
    int diffRiga = rigaArrivo - rigaPartenza;
    int absCol = abs(diffCol);
    int absRiga = abs(diffRiga);

    if (isPezzoMio(colArrivo, rigaArrivo)) return false;

    if (pezzo == "♔" || pezzo == "♚") if (absCol <= 1 && absRiga <= 1) return true;
    if (pezzo == "♘" || pezzo == "♞") if (absCol * absRiga == 2) return true;
    if (pezzo == "♖" || pezzo == "♜") if (diffCol == 0 || diffRiga == 0) if (stradaLibera(colPartenza, rigaPartenza, diffCol, diffRiga)) return true;
    if (pezzo == "♗" || pezzo == "♝") if (absCol == absRiga) if (stradaLibera(colPartenza, rigaPartenza, diffCol, diffRiga)) return true;
    if (pezzo == "♕" || pezzo == "♛") if (absCol == absRiga || diffCol == 0 || diffRiga == 0) if (stradaLibera(colPartenza, rigaPartenza, diffCol, diffRiga)) return true;

    if (pezzo == "♙") {
        if (diffRiga == -1 && diffCol == 0 && scacchiera[rigaArrivo][colArrivo] == " ") return true;
        if (rigaPartenza == 6 && diffRiga == -2 && diffCol == 0 && stradaLibera(colPartenza, rigaPartenza, diffCol, diffRiga) && scacchiera[rigaArrivo][colArrivo] == " ") return true;
        if (diffRiga == -1 && absCol == 1 && scacchiera[rigaArrivo][colArrivo] != " ") return true;
        //en passant
        if (diffRiga == -1 && absCol == 1 && scacchiera[rigaArrivo][colArrivo] == " " &&
            rigaPartenza == 3 && pedoneDoppioPassoRiga == rigaArrivo + 1 && pedoneDoppioPassoCol == colArrivo) return true;
    }

    if (pezzo == "♟") {
        if (diffRiga == 1 && diffCol == 0 && scacchiera[rigaArrivo][colArrivo] == " ") return true;
        if (rigaPartenza == 1 && diffRiga == 2 && diffCol == 0 && stradaLibera(colPartenza, rigaPartenza, diffCol, diffRiga) && scacchiera[rigaArrivo][colArrivo] == " ") return true;
        if (diffRiga == 1 && absCol == 1 && scacchiera[rigaArrivo][colArrivo] != " ") return true;
        //en passant
        if (diffRiga == 1 && absCol == 1 && scacchiera[rigaArrivo][colArrivo] == " " &&
            rigaPartenza == 4 && pedoneDoppioPassoRiga == rigaArrivo - 1 && pedoneDoppioPassoCol == colArrivo) return true;
    }

    return false;
}

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    while (true) {
        stampaScacchiera();

        if (!ilReEsiste("♔")) { cout << " VITTORIA DEL NERO! Il Re Bianco e' caduto." << endl; break; }
        if (!ilReEsiste("♚")) { cout << " VITTORIA DEL BIANCO! Il Re Nero e' caduto." << endl; break; }

        if (reSottoAttacco()) {
            cout << "\nATTENZIONE: IL TUO RE E' SOTTO SCACCO" << endl;
            cout << "Devi muovere il Re o proteggerlo, altrimenti perdi." << endl;
        }

        cout << endl;
        if (turnoGiocatore % 2 != 0) cout << "Turno del giocatore BIANCO";
        else cout << "Turno del giocatore NERO";

        cout << " (es: a2a4): ";
        string inputMossa;
        cin >> inputMossa;

        if (inputMossa.length() < 4) {
            pulisciSchermo();
            cout << "Errore: Mossa troppo corta!--Guarda l'esempio" << endl;
            continue;
        }

        int colPartenza = inputMossa[0] - 'a';
        int rigaPartenza = 7 - (inputMossa[1] - '1');
        int colArrivo = inputMossa[2] - 'a';
        int rigaArrivo = 7 - (inputMossa[3] - '1');

        bool arrocco = false;

        //Arrocco corto per le pedine bianche
        if (turnoGiocatore % 2 != 0) {
            if (colPartenza == 4 && rigaPartenza == 7 && colArrivo == 6 && rigaArrivo == 7 &&
                !reBiancoMosso && !torreH1Mossa &&
                scacchiera[7][4] == "♔" && scacchiera[7][7] == "♖" &&
                stradaLibera(4, 7, 3, 0) && !reSottoAttacco()) {
                scacchiera[7][4] = " ";
                scacchiera[7][5] = "♔";
                if (!reSottoAttacco()) {
                    scacchiera[7][5] = " ";
                    scacchiera[7][6] = "♔";
                    if (!reSottoAttacco()) arrocco = true;
                }
                scacchiera[7][6] = " ";
                scacchiera[7][4] = "♔";
            }
        }

        //Arrocco corto per le pedine nere
        if (turnoGiocatore % 2 == 0) {
            if (colPartenza == 4 && rigaPartenza == 0 && colArrivo == 6 && rigaArrivo == 0 &&
                !reNeroMosso && !torreH8Mossa &&
                scacchiera[0][4] == "♚" && scacchiera[0][7] == "♜" &&
                stradaLibera(4, 0, 3, 0) && !reSottoAttacco()) {
                scacchiera[0][4] = " ";
                scacchiera[0][5] = "♚";
                if (!reSottoAttacco()) {
                    scacchiera[0][5] = " ";
                    scacchiera[0][6] = "♚";
                    if (!reSottoAttacco()) arrocco = true;
                }
                scacchiera[0][6] = " ";
                scacchiera[0][4] = "♚";
            }
        }

        if (arrocco) {
            if (turnoGiocatore % 2 != 0) {
                scacchiera[7][6] = "♔";
                scacchiera[7][5] = "♖";
                scacchiera[7][4] = " ";
                scacchiera[7][7] = " ";
                reBiancoMosso = true;
                torreH1Mossa = true;
            } else {
                scacchiera[0][6] = "♚";
                scacchiera[0][5] = "♜";
                scacchiera[0][4] = " ";
                scacchiera[0][7] = " ";
                reNeroMosso = true;
                torreH8Mossa = true;
            }
            turnoGiocatore++;
            pulisciSchermo();
            continue;
        }

        
        if (colPartenza >= 0 && colPartenza < 8 && rigaPartenza >= 0 && rigaPartenza < 8 &&
            colArrivo >= 0 && colArrivo < 8 && rigaArrivo >= 0 && rigaArrivo < 8) {

            if (isPezzoMio(colPartenza, rigaPartenza)) {
                if (mossaValida(colPartenza, rigaPartenza, colArrivo, rigaArrivo)) {

                    string pezzo = scacchiera[rigaPartenza][colPartenza];

                    if (pezzo == "♔") reBiancoMosso = true;
                    if (pezzo == "♚") reNeroMosso = true;
                    if (pezzo == "♖" && rigaPartenza == 7 && colPartenza == 0) torreA1Mossa = true;
                    if (pezzo == "♖" && rigaPartenza == 7 && colPartenza == 7) torreH1Mossa = true;
                    if (pezzo == "♜" && rigaPartenza == 0 && colPartenza == 0) torreA8Mossa = true;
                    if (pezzo == "♜" && rigaPartenza == 0 && colPartenza == 7) torreH8Mossa = true;

                    //en passant
                    if (pezzo == "♙") {
                        if (abs(colArrivo - colPartenza) == 1 && rigaArrivo == rigaPartenza - 1 &&
                            scacchiera[rigaArrivo][colArrivo] == " " &&
                            pedoneDoppioPassoRiga == rigaArrivo + 1 && pedoneDoppioPassoCol == colArrivo) {
                            scacchiera[pedoneDoppioPassoRiga][pedoneDoppioPassoCol] = " ";
                        }
                    }

                    if (pezzo == "♟") {
                        if (abs(colArrivo - colPartenza) == 1 && rigaArrivo == rigaPartenza + 1 &&
                            scacchiera[rigaArrivo][colArrivo] == " " &&
                            pedoneDoppioPassoRiga == rigaArrivo - 1 && pedoneDoppioPassoCol == colArrivo) {
                            scacchiera[pedoneDoppioPassoRiga][pedoneDoppioPassoCol] = " ";
                        }
                    }
                    //Esecuzione mossa
                    scacchiera[rigaArrivo][colArrivo] = pezzo;
                    scacchiera[rigaPartenza][colPartenza] = " ";

                    //doppio passo aggiornato solo se il pedone si muove di due caselle
                    if (pezzo == "♙" && rigaPartenza == 6 && rigaArrivo == 4) {
                        pedoneDoppioPassoRiga = rigaArrivo;
                        pedoneDoppioPassoCol = colArrivo;
                    } else if (pezzo == "♟" && rigaPartenza == 1 && rigaArrivo == 3) {
                        pedoneDoppioPassoRiga = rigaArrivo;
                        pedoneDoppioPassoCol = colArrivo;
                    } else {
                        pedoneDoppioPassoRiga = -1;
                        pedoneDoppioPassoCol = -1;
                    }
                    //controllo promozione dei pedoni
                    promozionePedone(colArrivo, rigaArrivo);

                    turnoGiocatore++;

                    pulisciSchermo();

                } else {
                    pulisciSchermo();
                    cout << "Mossa non valida!" << endl;
                }
            } else {
                pulisciSchermo();
                cout << "Pezzo non tuo! oppure hai selezionato uno spazio vuoto!" << endl;
            }
        } else {
            pulisciSchermo();
            cout << "Coordinate errate!" << endl;
        }
    }

    return 0;
}
