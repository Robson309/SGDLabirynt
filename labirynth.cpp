#include <iostream>
#include "blocks\display.h"
#include "labirynthimpl.h"

void main(){

	try{
		Display().setTitle( "Labirynth" );
		LabirynthImpl impl;
		while ( Display().running() ){
			
			if ( Display().LMB() ){
				impl.processLMB();
			} else if ( Display().RMB() ){
				impl.processRMB();
			}

			Display().clear();
			impl.draw();
			Display().present();

		}

	} catch ( const CImgIOException& e ){
		cout << e.what() << endl;
	}  
}
