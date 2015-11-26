//		**********************
//		EDROOM SERVICE LIBRARY
//		**********************



//   Copyright (C) 2005  Oscar Rodriguez Polo
//
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,USA.
//
//
//  Contact: opolo@aut.uah.es
//  November 2005


#ifndef __EDROOM_SERVICE_LIBRARY_H
#define __EDROOM_SERVICE_LIBRARY_H


// INCLUDES

#include "edroombp.h"


//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************


//FAMILIA DE PLANTILLA QUE PERMITEN RESERVAR UN ALMACEN DE MEMORIA PARA UN
//NUMERO DE ELEMENTOS DEL MISMO TIPO Y GESTIONAR ESA MEMORIA DINAMICAMENTE.


//*************************************************************************
// CLASE BASE PARA MANEJAR PUNTEROS A OBJETOS CREADOS CON LA PLANTILLA

// Clase base M_d

//							PROBADA EN manj_mem\proy_bor\manj_mem.ide


//*************************************************************************
// CLASE QUE REALIZA UN ALMACEN REDIMENSIONABLE Y GESTIONABLE DE FORMA
// DISTRIBUIDA

// template <class T > class Mem_din_R_D


//							PROBADA EN manj_mem\proy_bor\manj_mem.ide
							// falta probar mejoras


//*************************************************************************
// CLASE QUE REALIZA UN ALMACEN REDIMENSIONABLE PERO NO GESTIONABLE DE FORMA
// DISTRIBUIDA

// template <class T > class Mem_din_R


//*************************************************************************
// CLASE QUE REALIZA UN ALMACEN NO REDIMENSIONABLE Y NO GESTIONABLE DE FORMA
// DISTRIBUIDA

// template <class T > class Mem_din



//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************



#ifndef __EDROOM_Mem_din3
#define __EDROOM_Mem_din3

///////////////////////////INCLUDES///////////////////////////////


//*************************************************************************
// CLASE BASE PARA MANEJAR PUNTEROS A OBJETOS CREADOS CON LA PLANTILLA

// Clase base M_d

//							PROBADA EN manj_mem\proy_bor\manj_mem.ide



class M_d{
public:

#ifndef _EDROOM_IS_EMBEDDED_
	virtual int FreeData(void * &apunt)=0;

#else

	virtual int FreeData(void * apunt)=0;

#endif

 virtual ~M_d(){};

};


#ifndef _EDROOM_IS_EMBEDDED_


//*************************************************************************
// CLASE QUE REALIZA UN ALMACEN REDIMENSIONABLE Y GESTIONABLE DE FORMA
// DISTRIBUIDA

// template <class T > class Mem_din_R_D


//							PROBADA EN manj_mem\proy_bor\manj_mem.ide
							// falta probar mejoras


// El tipo T puede ser cualquiera que tenga definido constructor y destructor

template <class T > class Mem_din_R_D :public M_d
{
  private:

	Pr_SemaphoreRec semaforo; // Gestiona la exclusion mutua del acceso al bloque
	Pr_SemaphoreBin semIntegridad; // Se utiliza para gestionar que todos los
										// elementos del bloque han sido devueltos antes
										// de liberar la memoria.

	unsigned int dimension;            // numero de elementos del tipo T reservados

	unsigned char vacio;
	T   * bloque,*elem_dispo; // ap al bloque y ap a un elemento disponible
	unsigned char * libre;    // bloque de enteros asociado al bloque
									  // que permite marcar los elmentos disponibles

	unsigned int pos_sigui;	  // Guarda el indice del posible siguiente que este
									// libre.

	unsigned int num_propios;			// Numero de elementos propios disponibles
	unsigned int num_ajenos;			// Numero de elementos del heap que han sido
									// utilizados y todavia no se han devuelto

	unsigned char Destruc_bloqueado;  // Indica que se ha quedado bloqueado el destructor
									// en espera de que todos los elementos del bloque
									// que se han utilizado sean devueltos al mismo
									// Asi aseguramos que la gestion distribuida del bloque
									// se realiza correctamente

#ifdef _EDROOM_IS_DEBUG_

	char *name;

#endif


  public:

#ifdef _EDROOM_IS_DEBUG_

	Mem_din_R_D(unsigned int, char *name);

#else

	Mem_din_R_D(unsigned int);


#endif


	~Mem_din_R_D(void);

//************************************************************************
// PETICION DE APUNTADOR A UN ELEMENTO DE LA CLASE T QUE ESTE LIBRE
//************************************************************************
	T * AllocData(void);

	// Devuelve tambien el indice del elemento
	T * AllocData(unsigned int&);

//***************************************************************************
// PERMITE DEVOLVER UN ELEMENTO AL BLOQUE PARA QUE SE PUEDA DISPONER OTRA VEZ
// DE EL. RETORNA UN 1 SI EL ELEMENTO YA ESTABA DISPONIBLE DE ANTES Y POR TANTO
// HA HABIDO UN ERROR EN EL MANEJO DEL BLOQUE
//***************************************************************************

	int FreeData(void * &apunt);
	int FreeData(T * &apunt);

//***************************************************************************
// PERMITE CHEQUEAR SI ESTA VACIO EL POOL
//***************************************************************************

	unsigned char Empty(){return vacio;}
};




//*************************************************************************
// CLASE QUE REALIZA UN ALMACEN REDIMENSIONABLE PERO NO GESTIONABLE DE FORMA
// DISTRIBUIDA

// template <class T > class Mem_din_R


//							PROBADA EN manj_mem\proy_bor\manj_mem.ide
							// falta probar mejoras


// El tipo T puede ser cualquiera que tenga definido constructor y destructor



template <class T > class Mem_din_R :public M_d
{
  private:

  //Pr_SemaphoreRec semaforo; // Gestiona la exclusion mutua del acceso al bloque

  unsigned int dimension;            // numero de elementos del tipo T reservados

  unsigned char vacio;
  T   * bloque,*elem_dispo; // ap al bloque y ap a un elemento disponible
  unsigned char * libre;    // bloque de enteros asociado al bloque
									  // que permite marcar los elmentos disponibles

  unsigned int pos_sigui;	  // Guarda el indice del posible siguiente que este
									// libre.

  public:

	Mem_din_R(unsigned int);

	~Mem_din_R(void);

//************************************************************************
// PETICION DE APUNTADOR A UN ELEMENTO DE LA CLASE T QUE ESTE LIBRE
//************************************************************************
	T * AllocData(void);

	// Devuelve tambien el indice del elemento
	T * AllocData(unsigned int&);

//***************************************************************************
// PERMITE DEVOLVER UN ELEMENTO AL BLOQUE PARA QUE SE PUEDA DISPONER OTRA VEZ
// DE EL. RETORNA UN 1 SI EL ELEMENTO YA ESTABA DISPONIBLE DE ANTES Y POR TANTO
// HA HABIDO UN ERROR EN EL MANEJO DEL BLOQUE
//***************************************************************************

	int FreeData(void * &apunt);
	int FreeData(T * &apunt);

//***************************************************************************
// PERMITE CHEQUEAR SI ESTA VACIO EL POOL
//***************************************************************************

	unsigned char Empty(){return vacio;}
};


//*************************************************************************
// CONSTRUCTOR DE LA PLANTILLA, NECESITA EL TIPO BASE, EL NUMERO DE ELEMENTOS
// DEL BLOQUE
//*************************************************************************

 template <class T > Mem_din_R<T>::Mem_din_R(unsigned int dimen)//:semaforo("Semaforo Bloque")

	{

			unsigned int n;

			if (dimen==0)
				{
				vacio=1;
				bloque=NULL;
				}
			else
				{
				 vacio=0;

				 bloque=new T[dimen];  // RESERVA MEMORIA BLOQUE
				 libre =new unsigned char[dimen]; // RESERVA MEM PARA LAS MARCAS

				 for (n=0;n<dimen;n++)
					*(libre + n)=1;  // MARCA TODOS COMO DISPONIBLES
				 elem_dispo=bloque;
				 dimension=dimen;
				 pos_sigui=1%dimension;

				 }
	 }


//*************************************************************************
// DESTRUCTOR  DE LA PLANTILLA, NECESITA EL TIPO BASE, EL NUMERO DE ELEMENTOS
// DEL BLOQUE
//*************************************************************************


template <class T > Mem_din_R<T>::~Mem_din_R(void){

			  if (bloque)
				{
						delete [] bloque;
						delete [] libre;

				}

}




//************************************************************************
// PETICION DE APUNTADOR A UN ELEMENTO DE LA CLASE T QUE ESTE LIBRE
//************************************************************************
template <class T > T * Mem_din_R<T>::AllocData(void)
	  {
			T *nuevo;
			unsigned int contador,indice;

			//semaforo.Wait(); // PIDE ACCESO A MEM COMPARTIDA

			if (vacio){
				nuevo=new(T);
				}
			else
				{
				nuevo=elem_dispo;  // DEVUELVE EL APUNTADOR
				*(libre+(unsigned int)(elem_dispo-bloque))=0; // MARCA EL ELEMENTO

				// MIRA SI ESTA DISPONIBLE OTRO ELEMENTO,
				// Y SI NO QUEDA EL BLOQUE MARCADO COMO VACIO

				vacio=1;
				contador=0;
				elem_dispo=NULL;
				do
					{
					indice=(pos_sigui+contador)%dimension;   //indice donde busca

					if(*(libre+indice))
						{
						//ENCUENTRA UNO LIBRE

						elem_dispo=bloque+indice;
						vacio=0;
						}
					contador++;
					}
				while((contador<dimension)&&(vacio));

				pos_sigui=((indice+1)%dimension);

				} // fin del else
			//semaforo.Signal();
			return(nuevo);
		 }

//***************************************************************************
// Devuelve tambien el indice del elemento dentro del array

template <class T > T * Mem_din_R<T>::AllocData(unsigned int & ind_elem)
	  {
			T *nuevo;
			unsigned int contador,indice;
			//semaforo.Wait(); // PIDE ACCESO A MEM COMPARTIDA
			if (vacio){
				nuevo=new(T);
				ind_elem=UINT_MAX;
				}
			else
				{
				nuevo=elem_dispo;  // DEVUELVE EL APUNTADOR
				*(libre+(unsigned int)(elem_dispo-bloque))=0; // MARCA EL ELEMENTO
				ind_elem=(unsigned int)(elem_dispo-bloque);
				// MIRA SI ESTA DISPONIBLE OTRO ELEMENTO,
				// Y SI NO QUEDA EL BLOQUE MARCADO COMO VACIO

				vacio=1;
				contador=0;
				elem_dispo=NULL;
				do
					{
					indice=(pos_sigui+contador)%dimension;   //indice donde busca

					if(*(libre+indice))
						{
						//ENCUENTRA UNO LIBRE

						elem_dispo=bloque+indice;
						vacio=0;
						}
					contador++;
					}
				while((contador<dimension)&&(vacio));

				pos_sigui=((indice+1)%dimension);

				} // fin del else
			//semaforo.Signal();
			return(nuevo);
		 }

//***************************************************************************
// PERMITE DEVOLVER UN ELEMENTO AL BLOQUE PARA QUE SE PUEDA DISPONER OTRA VEZ
// DE EL. RETORNA UN 1 SI EL ELEMENTO YA ESTABA DISPONIBLE DE ANTES Y POR TANTO
// HA HABIDO UN ERROR EN EL MANEJO DEL BLOQUE
//***************************************************************************

// Pasando un puntero a void

template <class T > int Mem_din_R<T>::FreeData(void * &ap)
		{
		  int error;
		  T* apunt;

		  long posicion, dimensionLong;
		  unsigned int indice;

		  apunt=(T*)ap;

		  dimensionLong=dimension;
		  posicion = apunt-bloque;

		  //semaforo.Wait();  // PIDE ACCESO A LA MEMORIA COMPARTIDA

		  if ((posicion <0)||(posicion >= dimensionLong)){

				delete((T*)apunt);

			}
			else
			{

			indice=(unsigned int) posicion; //esta chequeado que es mayor que cero
													// y menor que dimension

				if (*(libre+indice))
					error=1;
				else
				{

				*(libre+indice)=1; // LO MARCA COMO DISPONIBLE
				if (vacio)            // SI ESTABA VACIO EL BLOQUE
									// LO COLOCA COMO A DISPONER
					elem_dispo=apunt;

				ap=NULL; // El puntero sale apuntando a NULL

				error=0;
				vacio=0;

				}
			}
		  //semaforo.Signal();
		  return (error);
		}

// Pasando un puntero a la clase

template <class T > int Mem_din_R<T>::FreeData(T * &apunt)
		{
		  int error;

		  long posicion, dimensionLong;
		  unsigned int indice;

		  dimensionLong=dimension;
		  posicion = apunt-bloque;

		  //semaforo.Wait();  // PIDE ACCESO A LA MEMORIA COMPARTIDA

		  if ((posicion <0)||(posicion >= dimensionLong)) {

				delete((T*)apunt);

			}
			else
			{

			indice=(unsigned int) posicion; //esta chequeado que es mayor que cero
													// y menor que dimension

				if (*(libre+indice))
					error=1;
				else
				{

				*(libre+indice)=1; // LO MARCA COMO DISPONIBLE



				if (vacio)            // SI ESTABA VACIO EL BLOQUE
									// LO COLOCA COMO A DISPONER
					elem_dispo=apunt;

				apunt=NULL; // El puntero sale apuntando a NULL

				error=0;
				vacio=0;

				}
			}

		  //semaforo.Signal();
		  return (error);
		}





//*************************************************************************
// CONSTRUCTOR DE LA PLANTILLA, NECESITA EL TIPO BASE, EL NUMERO DE ELEMENTOS
// DEL BLOQUE Y UN APUNTADOR A UN SEMAFORO QUE PROTEGE EL BLQUE COMPARTIDO
//*************************************************************************


 #ifdef _EDROOM_IS_DEBUG_

  template <class T > Mem_din_R_D<T>::Mem_din_R_D(unsigned int dimen, char * name_)


														:semaforo("Semaforo Bloque")
														,semIntegridad("Semaforo Integridad ",0){

														name=name_;
 #else

 template <class T > Mem_din_R_D<T>::Mem_din_R_D(unsigned int dimen)

														:semIntegridad(0){


 #endif


			unsigned int n;
			num_ajenos=0;
			if (dimen==0)
				{
				vacio=1;
				bloque=NULL;
				num_propios=0;
				}
			else
				{
				 vacio=0;

				 bloque=new T[dimen];  // RESERVA MEMORIA BLOQUE
				 libre =new unsigned char[dimen]; // RESERVA MEM PARA LAS MARCAS

				 for (n=0;n<dimen;n++)
					*(libre + n)=1;  // MARCA TODOS COMO DISPONIBLES
				 elem_dispo=bloque;
				 dimension=dimen;
				 pos_sigui=1%dimension;

				 num_propios=dimension;  //numero de elementos libres
				 Destruc_bloqueado=0;  //Inicializo esta variable
				 }
	 }


//*************************************************************************
// DESTRUCTOR DE LA PLANTILLA, NECESITA EL TIPO BASE
//*************************************************************************


 template <class T > Mem_din_R_D<T>::~Mem_din_R_D(void){

		int inspecc=0;

			  if (bloque)
				{
					do{
						semaforo.Wait(); //Capturo la exclusion mutua del manejador
								// de memoria


						Destruc_bloqueado=0; // No esta bloqueado el destructor

						if((num_propios==dimension)&&(num_ajenos==0)){
							inspecc=0;
							delete [] bloque;
							delete [] libre;
							semaforo.Signal();  // Libero la exclusion mutua
						}
						else{

							Destruc_bloqueado=1;
							semaforo.Signal();  // Libero la exclusion mutua


							//cout <<"\n" << name << " bloqueado\n";

							//cout <<"\nBloqueado\n";

							semIntegridad.Wait(); //Espero a que se devuelva algun
										// elemento mediante FreeData()

							//cout <<"\nDesBloqueado\n";

							inspecc=1;
									 }
					}while(inspecc);
				}


	}


//************************************************************************
// PETICION DE APUNTADOR A UN ELEMENTO DE LA CLASE T QUE ESTE LIBRE
//************************************************************************
template <class T > T * Mem_din_R_D<T>::AllocData(void)
	  {
			T *nuevo;
			unsigned int contador,indice;
			semaforo.Wait(); // PIDE ACCESO A MEM COMPARTIDA
			if (vacio){
				nuevo=new(T);
				num_ajenos++;
				}
			else
				{
				num_propios--;
				nuevo=elem_dispo;  // DEVUELVE EL APUNTADOR
				*(libre+(unsigned int)(elem_dispo-bloque))=0; // MARCA EL ELEMENTO

				// MIRA SI ESTA DISPONIBLE OTRO ELEMENTO,
				// Y SI NO QUEDA EL BLOQUE MARCADO COMO VACIO

				vacio=1;
				contador=0;
				elem_dispo=NULL;
				do
					{
					indice=(pos_sigui+contador)%dimension;   //indice donde busca

					if(*(libre+indice))
						{
						//ENCUENTRA UNO LIBRE

						elem_dispo=bloque+indice;
						vacio=0;
						}
					contador++;
					}
				while((contador<dimension)&&(vacio));

				pos_sigui=((indice+1)%dimension);

				} // fin del else
			semaforo.Signal();
			return(nuevo);
		 }

//***************************************************************************
// Devuelve tambien el indice del elemento dentro del array

template <class T > T * Mem_din_R_D<T>::AllocData(unsigned int & ind_elem)
	  {
			T *nuevo;
			unsigned int contador,indice;
			semaforo.Wait(); // PIDE ACCESO A MEM COMPARTIDA
			if (vacio){
				nuevo=new(T);
				ind_elem=UINT_MAX;
				num_ajenos++;
				}
			else
				{
				num_propios--;
				nuevo=elem_dispo;  // DEVUELVE EL APUNTADOR
				*(libre+(unsigned int)(elem_dispo-bloque))=0; // MARCA EL ELEMENTO
				ind_elem=(unsigned int)(elem_dispo-bloque);
				// MIRA SI ESTA DISPONIBLE OTRO ELEMENTO,
				// Y SI NO QUEDA EL BLOQUE MARCADO COMO VACIO

				vacio=1;
				contador=0;
				elem_dispo=NULL;
				do
					{
					indice=(pos_sigui+contador)%dimension;   //indice donde busca

					if(*(libre+indice))
						{
						//ENCUENTRA UNO LIBRE

						elem_dispo=bloque+indice;
						vacio=0;
						}
					contador++;
					}
				while((contador<dimension)&&(vacio));

				pos_sigui=((indice+1)%dimension);

				} // fin del else
			semaforo.Signal();
			return(nuevo);
		 }

//***************************************************************************
// PERMITE DEVOLVER UN ELEMENTO AL BLOQUE PARA QUE SE PUEDA DISPONER OTRA VEZ
// DE EL. RETORNA UN 1 SI EL ELEMENTO YA ESTABA DISPONIBLE DE ANTES Y POR TANTO
// HA HABIDO UN ERROR EN EL MANEJO DEL BLOQUE
//***************************************************************************

// Pasando un puntero a void

template <class T > int Mem_din_R_D<T>::FreeData(void * &ap)
		{
		  int error;
		  T* apunt;

		  long posicion;
		  long dimensionLong;
		  unsigned int indice;

		  apunt=(T*)ap;

		  posicion = apunt-bloque;

		  dimensionLong=dimension;


		  semaforo.Wait();  // PIDE ACCESO A LA MEMORIA COMPARTIDA

		  if ((posicion <0)||(posicion >= dimensionLong)){

				delete((T*)apunt);
				num_ajenos--;
				error=0;
			}
			else
			{

			indice=(unsigned int) posicion; //esta chequeado que es mayor que cero
													// y menor que dimension

				if (*(libre+indice))
					error=1;
				else
				{
				num_propios++;
				*(libre+indice)=1; // LO MARCA COMO DISPONIBLE



				if (vacio)            // SI ESTABA VACIO EL BLOQUE
									// LO COLOCA COMO A DISPONER
					elem_dispo=apunt;

				ap=NULL; // El puntero sale apuntando a NULL

				error=0;
				vacio=0;

				}
			}
			if (Destruc_bloqueado&&(!error)){
					semIntegridad.Signal();
					Destruc_bloqueado=0;
			}

		  semaforo.Signal();
		  return (error);
		}

// Pasando un puntero a la clase

template <class T > int Mem_din_R_D<T>::FreeData(T * &apunt)
		{
		  int error;

		  long posicion,dimensionLong;
		  unsigned int indice;

		  posicion = apunt-bloque;

		  semaforo.Wait();  // PIDE ACCESO A LA MEMORIA COMPARTIDA

		  dimensionLong=dimension;

		  if ((posicion <0)||(posicion >= dimensionLong)) {

				delete((T*)apunt);
				num_ajenos--;
				error=0;
			}
			else
			{

			indice=(unsigned int) posicion; //esta chequeado que es mayor que cero
													// y menor que dimension

				if (*(libre+indice))
					error=1;
				else
				{
				num_propios++;
				*(libre+indice)=1; // LO MARCA COMO DISPONIBLE



				if (vacio)            // SI ESTABA VACIO EL BLOQUE
									// LO COLOCA COMO A DISPONER
					elem_dispo=apunt;

				apunt=NULL; // El puntero sale apuntando a NULL

				error=0;
				vacio=0;

				}
			}
			if (Destruc_bloqueado&&(!error)){
					semIntegridad.Signal();
					Destruc_bloqueado=0;
			}

		  semaforo.Signal();
		  return (error);
		}




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#else

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CLASE QUE REALIZA UN ALMACEN REDIMENSIONABLE PERO NO GESTIONABLE DE FORMA
// DISTRIBUIDA

// class M_d_R


//							PROBADA EN manj_mem\proy_bor\manj_mem.ide
							// falta probar mejoras



class M_d_R: public M_d{

  protected:

  //Pr_SemaphoreRec semaforo; // Gestiona la exclusion mutua del acceso al bloque

  unsigned int dimension;            // numero de elementos del tipo EDROOMMessage reservados

  unsigned char vacio;

  EDROOMByte   * bloque,*elem_dispo; // ap al bloque y ap a un elemento disponible

  unsigned int size;

  unsigned char * libre;    // bloque de enteros asociado al bloque
									  // que permite marcar los elmentos disponibles

  unsigned int ind_dispo;	  // Guarda el indice del elemento disponible.

  public:


  void SetMemory(unsigned int dimension, void * Memory, unsigned char * MarksMemory,  unsigned int size);


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PETICION DE APUNTADOR A UN ELEMENTO DE LA CLASE ROOMMessage QUE ESTE LIBRE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	EDROOMByte * AllocData(void);

	// Devuelve tambien el indice del elemento
	EDROOMByte * AllocData(unsigned int&);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	int FreeData(void * apunt);


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PERMIROOMMessageE CHEQUEAR SI ESROOMMessageA VACIO EL POOL
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	unsigned char Empty(){return vacio;}



};


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CLASE QUE REALIZA UN ALMACEN REDIMENSIONABLE Y GESTIONABLE DE FORMA
// DISTRIBUIDA

// template <class T > class Mem_din_R_D


//							PROBADA EN manj_mem\proy_bor\manj_mem.ide
							// falta probar mejoras


// El tipo T puede ser cualquiera que tenga definido constructor y destructor


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CONSTRUCTOR DE LA PLANTILLA, NECESITA EL TIPO BASE, EL NUMERO DE ELEMENTOS
// DEL BLOQUE Y UN APUNTADOR A UN SEMAFORO QUE PROTEGE EL BLQUE COMPARTIDO

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


class M_d_R_D :public M_d_R
{
  private:

	Pr_SemaphoreRec semaforo; // Gestiona la exclusion mutua del acceso al bloque


  public:

	M_d_R_D(unsigned int dimension, void * Memory,  unsigned char * MarksMemory, unsigned int size);

	void SetMemory(unsigned int dimension, void * Memory,  unsigned char * MarksMemory, unsigned int size);


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PETICION DE APUNTADOR A UN ELEMENTO DE LA CLASE T QUE ESTE LIBRE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	EDROOMByte * AllocData(void);

	// Devuelve tambien el indice del elemento
	EDROOMByte * AllocData(unsigned int&);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PERMITE DEVOLVER UN ELEMENTO AL BLOQUE PARA QUE SE PUEDA DISPONER OTRA VEZ
// DE EL. RETORNA UN 1 SI EL ELEMENTO YA ESTABA DISPONIBLE DE ANTES Y POR TANTO
// HA HABIDO UN ERROR EN EL MANEJO DEL BLOQUE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	int FreeData(void * apunt);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PERMITE CHEQUEAR SI ESTA VACIO EL POOL
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	unsigned char Empty(){return vacio;}
};




#endif


#endif


 
 

#ifndef __EDROOM_TYPES_H
#define __EDROOM_TYPES_H

//**************************************************************************
//**************************************************************************
//**************************************************************************
// EdROOMTYPES
//**************************************************************************
//**************************************************************************
//**************************************************************************

//  +++++++++++++++
// EdROOMActor Types

class EDROOMActor;

#ifdef _EDROOM_IS_EMBEDDED_

class EDROOMActorMemory;

#endif

//  +++++++++++++++
//  EDROOMST Types

// Tipo que se exporta tambien a los fichero roommes.h

enum EDROOMTimerSt{edroomTimerCancelled,edroomTimerReady, edroomTimerActive, edroomTimerSent, edroomTimerDummy};  //  Tipo que almacena si el timer ha sido
												//  o no cancelado
class EDROOMTimerInf;



class EDROOMConnection; //clases que exporta
class EDROOMConnection; //clases que exporta

class EDROOMConnectionChannel;
class EDROOMCommSAP;

#ifdef _EDROOM_IS_EMBEDDED_
class M_d_R_EDROOMConnection;

#else
typedef Mem_din_R<EDROOMConnection> M_d_R_EDROOMConnection;

#endif


//  +++++++++++++++
//  EDROOMMESS Types

// Declaracion de las clases que se usan en otros modulos

class EDROOMMessage;
class EDROOMSynchrMessage;
class EDROOMTimeOutMessage;
class EDROOMSynchrMessageManager;

#ifdef _EDROOM_IS_EMBEDDED_

class M_d_R_EDROOMMessage;
class M_d_R_EDROOMTimeOutMessage;

#else

typedef Mem_din_R<EDROOMMessage> M_d_R_EDROOMMessage;
typedef Mem_din_R<EDROOMTimeOutMessage> M_d_R_EDROOMTimeOutMessage;

#endif

typedef int EDROOMSignal;

//const EDROOMSignal EDROOMNoSignal=-1;

#define EDROOMNoSignal  -1

//  +++++++++++++++
//  EDROOMINTE Types


class EDROOMInterface;

//  +++++++++++++++
//  EDROOMCOMMSAP Types



class EDROOMConnection; //clases que exporta
class EDROOMConnection; //clases que exporta

class EDROOMConnectionChannel;
class EDROOMCommSAP;

#ifdef _EDROOM_IS_EMBEDDED_
class M_d_R_EDROOMConnection;

#else
typedef Mem_din_R<EDROOMConnection> M_d_R_EDROOMConnection;

#endif


//**************************************************************************
//**************************************************************************
//**************************************************************************
// FIN EdROOMTYPES
//**************************************************************************
//**************************************************************************
//**************************************************************************

#endif
 
 
#ifndef __EDROOM_MESSAGE_H
#define __EDROOM_MESSAGE_H


//**************************************************************************
// IMPLEMENTACION DE LOS MENSAJES DE UN MODELO EDROOM BASADAS EN LAS
// PRIMITIVAS DE TIEMPO REAL DEFINIDAS EN prim_pp.h

// Clase base EDROOMMessage       PROBADA:
//                                                                                                                en EDROOM_QU\proy_bor\queue.ide

// Clases derivadas EDROOMSynchrMessage
//                  EDROOMTimeOutMessage
//**************************************************************************


//***************************
// Struct EDROOMTransId

  struct EDROOMTransId{
		int name;
		int contextLevel;
  };


//***********************************************************************
// Class EDROOMMessage


class EDROOMMessage
{

	friend class EDROOMActor;
	friend class EDROOMQueue;
	friend class EDROOMInterface;
	friend class EDROOMIRQInterface;
	friend class EDROOMConnectionChannel;

// Funcion amiga para liberar la memoria de los mensajes
// Sustituye al delete tradicional

friend void EDROOMFreeMessage(EDROOMMessage *&);
friend void EDROOMFreeMsgBack(EDROOMMessage *&);

private:

public: //Sacado a public momentaneamente

	M_d             *pPoolMess; //apuntador al manejador de memoria de los Mensajes
	M_d             *pPoolData; //apuntador al manejador de memoria de los datos


protected:

	bool virtual cancelled();

public:

	EDROOMInterface   * edroominterface;    // Puntero al interface que saca el mensaje


public:

	Pr_Time   		time;    			// Tiempo en el que fue enviado el mensaje
	EDROOMSignal      signal;         //message signal
	EDROOMPriority    priority;       //message priority
	void            *data;          //pointer to optional data


	EDROOMInterface   * EDROOMinterface (); //interface component on wich message arrived

	void virtual reply(EDROOMSignal);
	void virtual reply(EDROOMSignal, void * p_data,M_d *pPoolDat);
	void virtual reply(EDROOMMessage * Msg);
	void virtual reply(EDROOMSignal, EDROOMMessage * Msg);


  //*************** CONSTRUCTOR INLINE **********************************

	EDROOMMessage();

   	//*************** DESTRUCTOR INLINE **********************************

	virtual ~EDROOMMessage(){};


};





class EDROOMSynchrMessage:public EDROOMMessage
{

 friend class EDROOMInterface;
 friend class EDROOMConnectionChannel;

protected:

	Pr_SemaphoreBin         semSynchr; //Semaforo para la sincronizacion del mensaje


	EDROOMMessage     *p_MsgBack; //apuntador al mensaje de retorno

	bool             waiting;         //indica si esta esperando o no el emisor


public:

	void virtual reply(EDROOMSignal);
	void virtual reply(EDROOMSignal, void *, M_d *apManMemDat);
   void virtual reply(EDROOMMessage * Msg);
	void virtual reply(EDROOMSignal, EDROOMMessage * Msg);

	// CONSTRUCTOR

	EDROOMSynchrMessage();

   
	// DESTRUCTOR

	virtual ~EDROOMSynchrMessage(){};

};




class EDROOMTimeOutMessage:public EDROOMMessage
{

friend class EDROOMTimingSAP;
friend class EDROOMActor;

protected:

	bool virtual cancelled();

private:

	EDROOMTimerInf *pTimerInf;         //Apuntador al elemento que determina el estado
										//del timer

	M_d *pPoolTimerInf;      // Apuntador al bloque de memoria utilizado para
										// reservar los elementos de tipo EDROOMTimerSt

	Pr_SemaphoreRec *pMutexSem;   // apuntador al Semaforo que protege el Servicio de
								  // Temporizacion


public:
	virtual ~EDROOMTimeOutMessage(){};


};




class EDROOMSynchrMessageManager:public M_d{

	Pr_SemaphoreBin         semMutex; //Semaforo para guardar la exclusion mutua

	EDROOMSynchrMessage *  SynchrMsg;

public:

	EDROOMSynchrMessageManager(EDROOMSynchrMessage *  SynchrMsg__);

	EDROOMSynchrMessage *  AllocData(void);

#ifdef _EDROOM_IS_EMBEDDED_


	int FreeData(void * apunt);

#else

	int FreeData(void * &apunt);

#endif

 virtual ~EDROOMSynchrMessageManager(){};

};



#ifdef _EDROOM_IS_EMBEDDED_

class M_d_R_EDROOMMessage: public M_d_R {

public:
		
		void SetMemory(unsigned int elemNumber, EDROOMMessage * pPoolData,unsigned char * pPoolMarks);


		EDROOMMessage * AllocData();


		virtual ~M_d_R_EDROOMMessage(){};
  };


class M_d_R_EDROOMTimeOutMessage: public M_d_R {

public:
		
		void SetMemory(unsigned int elemNumber, EDROOMTimeOutMessage* pPoolData,unsigned char * pPoolMarks);

  
		EDROOMTimeOutMessage * AllocData();

		virtual ~M_d_R_EDROOMTimeOutMessage(){};

};



#endif




#endif
 
 
//************************ PREGUNTAS ***********************************

//Seria preferible que se devolviera un entero en la funcion send, esto
//esto permitiria chequear si el envio utiliza memoria dinamica (en el caso
//que modifiquemos la cola)



#ifndef __EDROOM_Interface
#define __EDROOM_Interface


//***************************************************************************
//***********************     PROTOCOLO *************************************

		// La deteccion de la compatibilidad de protocolos se deja para
		// el editor grafico.
		// La implementacion del protocolo no es mas que un tipo enumerado
		// con los nombres de las se�les posibles del protocolo
		// El protocolo(tipo enumerado) esta definido dentro del ambito
		// de la clase actor en la que se encuentra el interfaz.
		// Los


#ifdef _EDROOM_IS_DEBUG_

enum EDROOMErrorComm  { EDROOMNoErrorComm, EDROOMEmptyPoolMens, EDROOMSignalInvalidate, EDROOMFullQueueMens,
							EDROOMInterfaceDisconnected, EDROOMCommTimeOut, EDROOMMsgBackUsed, EDROOMInterblock };

#endif


class EDROOMInterface{  // Para mensajes locales (Dentro del mismo computador)
		// Para mensajes no locales definiremos otra clase RemoteEDROOMInterface

friend class EDROOMCommSAP;
friend class EDROOMConnectionChannel;

protected:

	EDROOMActor* p_actor;

	// Conexion

	EDROOMConnectionChannel * p_channel;


#ifdef _EDROOM_IS_DEBUG_

	bool registered; 	// Indica si el puerto esta registrado o no.
	bool connected; // Indica si el puerto esta conectado o no.

#endif

	unsigned long idInterface;		// Identificador global del interface


public:

	//********************************************************************
	// Constructor 

#ifdef _EDROOM_IS_DEBUG_
	EDROOMInterface(); // solo para depuracion

	// asynchronous communication primitives:
	EDROOMErrorComm	send (EDROOMSignal signal, EDROOMPriority priority); // Se da la prioridad
	EDROOMErrorComm	send (EDROOMSignal signal, EDROOMPriority priority, void *dataObjetPtr, M_d * apMemDats);
	EDROOMErrorComm	send (EDROOMMessage *message);  // reenvio de un mensaje
	EDROOMErrorComm	send (EDROOMSignal signal,EDROOMMessage *message);  // reenvio de un mensaje
	EDROOMErrorComm	send (EDROOMSignal signal,EDROOMPriority priority,EDROOMMessage *message);  // reenvio de un mensaje


#else

	void	send (EDROOMSignal signal, EDROOMPriority priority); // Se da la prioridad
	void	send (EDROOMSignal signal, EDROOMPriority priority, void *dataObjetPtr, M_d * apMemDats);
	void	send (EDROOMMessage *message);  // reenvio de un mensaje
	void	send (EDROOMSignal signal,EDROOMMessage *message);  // reenvio de un mensaje
	void	send (EDROOMSignal signal,EDROOMPriority priority,EDROOMMessage *message);  // reenvio de un mensaje


#endif


	// synchronous communication primitives:


	EDROOMMessage *invoke(EDROOMSignal signal,EDROOMPriority priority=EDROOMprioURGENT);
	EDROOMMessage *invoke(EDROOMSignal signal, void *dataObjetPtr, M_d * apMemDats);
	EDROOMMessage *invoke(EDROOMSignal signal,EDROOMPriority priority, void *dataObjetPtr, M_d * apMemDats);

	EDROOMMessage *invoke(EDROOMMessage *message);  // reenvio de un mensaje
	EDROOMMessage *invoke(EDROOMSignal signal,EDROOMMessage *message);  // reenvio de un mensaje
	EDROOMMessage *invoke(EDROOMSignal signal,EDROOMPriority priority, EDROOMMessage *message);  // reenvio de un mensaje


	//identity test functions:

	int operator ==(EDROOMInterface &edroominterface);

	int operator !=(EDROOMInterface &edroominterface);

	EDROOMSignal applyConverFunct(EDROOMSignal signal);

	// recalling and purging deferred messages:



	#ifdef _EDROOM_IS_DEBUG_

	char * name;

	#endif



};

#ifdef _EDROOM_HANDLE_IRQS

class EDROOMIRQInterface: public EDROOMInterface{

	Pr_IRQHandler HandIRQ;
	Pr_IRQHandler HandIdle;
	Pr_SemaphoreBin * TerminarIRQLook;
//	Pr_SemaphoreBin * SemIRQLook;
	Pr_IRQEvent *IRQEventLook;
	EDROOMPriority priority;
	int IRQ;
	unsigned char IRQInstalled;

public:

	EDROOMIRQInterface(	Pr_IRQHandler HandIRQ_,	Pr_IRQHandler HandIdle_,	Pr_IRQEvent &IRQEventLook_ , Pr_SemaphoreBin & TerminarIRQLook_,	EDROOMPriority prio, int IRQ_);

	void	NewIRQMsg(EDROOMSignal signal, void * dataObjetPtr=NULL, M_d * pDataPool=NULL);

	void InstallHanler();

	void DeInstallHanler();

	void EndIRQInterm();

};

#endif



#endif
 
 
//**************** PREGUNTAS *****************************************

//Ahora hay un maximo de timers para cada SAP, cuando sobrepasas el limite
//no se reserva el timer.
//Esto se puede chequear puesto que el identificador del timer tiene una
//funcion miembro que dice si el timer es correcto o no.

//Se puede variar facilmente haciendo que no haya numero maximo de timers
//Esto pone en peligro las caracteristicas de T Real puesto que se utilizaria
//memoria dinamica
//La funcion correccion nos diria ahora si se ha utilizado mem dinamica o no



/* SERVICIO DE TEMPORIZACION DE LA MAQUINA VIRTUAL */

#ifndef __EDROOM_TIMMING_SERVICE
#define __EDROOM_TIMMING_SERVICE



//////////////////////DEFINES////////////////////////////////////////


#define timer_Stack       1024  //Tama� de la pila del timer.

// Tipos y estructuras definidas para el uso del Servicio de Temporizacion

class EDROOMTimerInf{

friend class   EDROOMTimingSAP;
friend class   EDROOMTimeOutMessage;

private:

	unsigned int indice; // Indice que sirve para identificar al Timer dentro del Servicio
					// de temporizacion

	Pr_Time timeId;  // Tiempo en el que fue reservado el Timer. Completa la identificacion

	Pr_Time time;  // Tiempo en el que esta programado el Timer

	EDROOMTimerInf *next;

	EDROOMTimerSt state;

	EDROOMTimeOutMessage *Msg;

public:

	// CONSTRUCTOR


};


#ifdef _EDROOM_IS_EMBEDDED_

class EDROOMTimmingMemory;

class M_d_R_EDROOMTimerInf: public M_d_R {


public:
		
		void SetMemory(unsigned int elemNumber, EDROOMTimerInf* pPoolData,unsigned char * pPoolMarks );
   
		EDROOMTimerInf * AllocData(unsigned int &ind);

};


#else

typedef Mem_din_R<EDROOMTimerInf> M_d_R_EDROOMTimerInf;

#endif



class EDROOMTimer{

friend class   EDROOMTimingSAP;

	unsigned int indice; // Indice que sirve para identificar al Timer dentro del Servicio
					// de temporizacion

	Pr_Time timeId;  // Tiempo en el que fue reservado el Timer. Completa la identificacion


	EDROOMTimerInf * timerInf; // Apunta a la informacion del timer

public:

	EDROOMTimer();

					// Devuelve un 1 si se ha reservado correctamente el Timer

	bool correcto();


};

class EDROOMTimerConfig {

friend class   EDROOMTimingSAP;

		Pr_Time time;

		EDROOMTimerInf * timerInf;


};



//***************************************************************************
//********************* class EDROOMTimingSAP ********************************

class EDROOMTimingSAP:public EDROOMInterface{


private:

	EDROOMActor * pActor;   // Apuntador al actor propietario del SAP


	//Sem�oro.

	Pr_SemaphoreRec mutexSem;   // Controla la exclusi� mutua del sevicio
									// de temporizaci�.

	Pr_SemaphoreBin synchrSem;


// memoria para manejar din�icamente los estados de los timer

	M_d_R_EDROOMTimerInf *timerInfPool;

	EDROOMTimerInf *nextTimer;

// memoria para manejar din�icamente los mensajes

	M_d_R_EDROOMTimeOutMessage *timeOutMsgPool;

	Pr_Task *ptimerTask;

	Pr_Task timerTask;

	Pr_SemaphoreBin waitActorSem;

#ifdef _EDROOM_SYSTEM_CLOSE

	bool SAPFinished;

#endif


public:


	// CONSTRUCTOR

#ifndef _EDROOM_IS_EMBEDDED_

	EDROOMTimingSAP(EDROOMActor * p_Actor_,unsigned int Num_max_timers);

#else


	EDROOMTimingSAP(EDROOMActor * p_Actor_,unsigned int Num_max_timers, EDROOMTimmingMemory *pTimmingMemory);

#endif




	// DESTRUCTOR


#ifdef _EDROOM_SYSTEM_CLOSE

	~EDROOMTimingSAP();

#endif


	// absolute time functions:


	void currentTime(Pr_Time &time);

	Pr_Time currentTime();


	EDROOMTimer InformAt(Pr_Time timeValue,EDROOMPriority MsgPrio);


	EDROOMTimer InformAt(Pr_Time timeValue, EDROOMPriority MsgPrio, void *data,
																M_d * bloqueMemData);

	// relative timing functions:


	EDROOMTimer InformIn  (Pr_Time timeoutValue,EDROOMPriority MsgPrio);

	EDROOMTimer InformIn  (Pr_Time timeoutValue, EDROOMPriority MsgPrio, void *data,
																M_d * bloqueMemData);

	// canceling outstanding timers:

	void  cancelTimer(EDROOMTimer &timerId);

	// Funcion que realiza el timer

  #ifdef _EDROOM_TASK_POSIX_

    static void * EDROOMfunTimer(void * paramDumy);

  #else

    #ifdef _EDROOM_TASK_ONE_PARAM_VOID

    static void EDROOMfunTimer(void * paramDumy);

    #else

    static void EDROOMfunTimer(void);

    #endif

  #endif



	void  Start();


};


#ifdef _EDROOM_IS_EMBEDDED_


class EDROOMTimmingMemory{

friend class EDROOMTimingSAP;

	public:


	  M_d_R_EDROOMTimerInf  TimerInfMem;

	  M_d_R_EDROOMTimeOutMessage  TimeOutMessMem;


	  void SetMemory(unsigned int elemNumber, EDROOMTimerInf * TimerInfMem, unsigned char *TimerInfMemMarks, EDROOMTimeOutMessage* TimeOutMessageMem, unsigned char *TimeOutMessageMemMarks);


};



#endif



#endif
 
 
//*********************************************************************
//**************                        PREGUNTAS *************************************
// Ahora hay un numero finito de mensajes a recibir, se puede eliminar
// ese numero finito, esto haria que se emplease memoria dinamica que no
// es de tiempo real.



//**********************************************************************
// IMPLEMENTACION DE LA COLA DE MENSAJES DEL SERVICIO DE COMUNICACIONES
// DE LA MAQUINA VIRTUAL DE ROOM
//


// Clase ROOMQueue PROBADO en edroom_qu\proy_bor\queue.ide

//                        para una sola prioridad
//                        correcto manejo de memoria
//                        muchas prioridades distintas
//                        Las funciones miembro:
//                                                                                      In
//                                                                                      Out
//                               InDefer
//                                                                                      recall
//                                                                                      recallALL

//                                                                                                               en ROOM_QU\proy_bor\queue.ide

//               FALTA
//                         probar la funcion miembro OutCond

//**********************************************************************





#ifndef __EDROOM_QUEUE_H
#define __EDROOM_QUEUE_H

//************************************************************************
//*************************** CLASE EDROOMQueue ******************************



class EDROOMQueue
{

public:

  class QueueNode{               // Nodo con la informaci� del mensaje
	friend class EDROOMQueue;

	EDROOMMessage * p_Msg;
	QueueNode *p_next;

	public:

	//constructor  INLINE
	QueueNode();

  };



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef _EDROOM_IS_EMBEDDED_

   class M_d_R_QueueNode: public M_d_R {

	friend class EDROOMActorMemory;

	   public:
	
		
		void SetMemory(unsigned int elemNumber, QueueNode * pPoolData,unsigned char * pPoolMarks);

 	 
		QueueNode * AllocData();

  };

#else

	typedef Mem_din_R<QueueNode> M_d_R_QueueNode;

#endif


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  class QueueHead{            // Cabeza de las colas

	friend class EDROOMQueue;
												// Guarda la prioridad de esa cola
	EDROOMPriority priority;
	QueueNode *p_first;
	QueueNode *p_last;
	QueueHead *p_next;
	//constructor
	public:
	QueueHead();
  };

public:


// ****************************************************************************

/////////////////DECLARACION FUNCIONES MIEMBRO PUBLICAS///////////////////////

// ****************************************************************************



// ****************************************************************************
///////////////DECLARACION DEL CONSTRUCTOR ////////////////////////////////////
// ****************************************************************************


#ifdef _EDROOM_IS_EMBEDDED_

	EDROOMQueue(unsigned  Num_Max_Men, EDROOMActor  *p_actor_,      unsigned num_prio_   ,EDROOMActorMemory *pActorMemory);

#else

	EDROOMQueue(unsigned  Num_Max_Men, EDROOMActor  *p_actor_,      unsigned num_prio_ );

#endif


#ifdef _EDROOM_SYSTEM_CLOSE

// ****************************************************************************
///////////////DECLARACION DEL DESTRUCTOR ////////////////////////////////////
// ****************************************************************************

	~EDROOMQueue();

// ****************************************************************************
///////////////LIBERA COLA DESTRUCTOR ////////////////////////////////////
// ****************************************************************************

	void FreeMem();


#endif


// ****************************************************************************
///////////////FUNCION MIEMBRO   LLena ////////////////////////////////////
// ****************************************************************************
	bool Full();

// ****************************************************************************
///////////////FUNCION MIEMBRO   In ////////////////////////////////////
// ****************************************************************************

// Mete un mensaje en la cola

#ifdef _EDROOM_IS_DEBUG_

	bool In (EDROOMMessage* p_Msg);  // La prioridad del mensaje va
											 // en el propio mensaje
											 // Devuelve un 0 si no hay error

#else

	void In (EDROOMMessage* p_Msg);  // La prioridad del mensaje va

#endif

// ****************************************************************************
///////////////FUNCION MIEMBRO   Out ////////////////////////////////////
// ****************************************************************************

// Saca un mensaje de la cola

	bool Out(EDROOMMessage * &);      // Espera hasta que haya un menjaje en la cola

 //	unsigned char OutCond(EDROOMMessage * &);   // No espera. Devuelve un 1 si hay mensaje y un
					// 0 si no lo hay.

// *********************************************************************************
///////////////FUNCION MIEMBRO   GetMaxPriority ////////////////////////////////////
// *********************************************************************************


	void SetToMsgPriority(void);



protected:

////////////////////////// CAMPOS PROTEGIDOS //////////////////////////////////

	EDROOMActor        *p_actor;  // Tarea que realiza al actor propietario de la cola

	unsigned num_prio;       // Numero de prioridades.

	unsigned max_num_mess;   // Numero maximo de mensajes que se pueden meter
									 // directamente en la cola, no se cuentan los
									 // urgentes ni los diferidos reencolados

	unsigned num_mess;       // Numero de mensajes en cola metidos directamente.


	//Pr_SemaphoreRec         &mutexSem;   // Semaforo que asegura la exclusi� mutua de
											//la cola

	//Pr_SemaphoreRec         &semaDifer;   // Semaforo que asegura la exclusi� mutua de
											//la cola de Diferidos

	Pr_SemaphoreBin 		newMessSem; // Semaforo en el que se queda esperando
												//la tarea propietaria cuando quiere sacar un
												// mensaje y no hay ninguno en la cola



	int bloqueada;               // Flag que determina si esta bloqueada la tarea
						  // propietaria para despertarla


	  // apuntador al bloque de memoria de las cabezas
	  // Puede cambiar la dimension del bloque _R


	QueueHead  * queueHeads;
	QueueHead  * p_auxQueue;


	  // apuntador al bloque de memoria de los nodos
	  // Puede cambiar la dimension del bloque _R

	M_d_R_QueueNode   *p_mem_nodes;


// ****************************************************************************

/////////////////DECLARACION FUNCIONES MIEMBRO PROTEGIDAS///////////////////////

// ****************************************************************************
	void UpDateAuxQueue(EDROOMPriority priority);


		// Funcion para meter en una cola un nodo.

	bool PutInNode(QueueHead *head , QueueNode *);

		// Funcion que saca el mesaje de m� prioridad que hay en la cola.

	void TakeOutNode(QueueNode * &p_node, EDROOMPriority &priority); //Saco de colas


};




#endif

 
 

// Los protocolos son en la implementacion simples enumerados que tienen como
// valor de la se�l el numero de bytes del tipo que transportan
// El chequeo de la compatibilidad de protocolos creo que debera hacerse en
// el editor grafico quitando complejidad de procesamiento

#ifndef __EDROOM_CSAP_H
#define __EDROOM_CSAP_H

// SAP DEL SERVICIO DE COMUNICACIONES DE LA MAQUINA VIRTUAL DE EDROOM


// class EDROOMConnection
// class EDROOMCOMMSAP

//**************************************************************************
//******************* class EDROOMConnection ***********************************

// Permite representar las conexiones entre los interfaces.



class EDROOMConnectionChannel{

friend class EDROOMCommSAP;
friend class EDROOMInterface;
friend class EDROOMConnection;

private:

	EDROOMInterface * p_senderPort;    //Los dos puertos que forman la conexion
	EDROOMInterface * p_receiverPort;

	// NUEVO

	// Apuntador a la tareas propietarias de los puertos

	Pr_Task  *p_senderTask;
   Pr_Task  *p_receiverTask;

	// Apuntador a la cola de mensajes del los actores propietarios de los puertos

	EDROOMQueue *p_QueueReceiver;


	// Apuntador a los manejadores de memoria de los mensajes

	M_d_R_EDROOMMessage *p_MsgPoolReceiver;

	EDROOMMessage ** p_p_MsgBack;
	EDROOMMessage * p_MessageBack;

	EDROOMSynchrMessageManager *p_SynchrMsgManager;

	//FUNCIONES DE CONVERSION DE SE�LES

	EDROOMSignal  (*converFunct) (EDROOMSignal );


public:

	//Constructor que inicializa a NULL todos los campos
	EDROOMConnectionChannel();
	// asynchronous communication primitives:

	void send (EDROOMSignal signal, EDROOMPriority priority, void *dataObjetPtr, M_d * apMemDats);

	// synchronous communication primitives:

	EDROOMMessage *invoke(EDROOMSignal signal, EDROOMPriority priority, void *dataObjetPtr, M_d * apMemDats);


};


class EDROOMConnection {

friend class EDROOMCommSAP;
friend class EDROOMInterface;


	EDROOMConnectionChannel channel1;
	EDROOMConnectionChannel channel2;

	M_d_R_EDROOMConnection *p_ConnectPool; //El bloque de memoria de la conexion

};

// Tipo para manejar las conexiones

typedef EDROOMConnection * ConnectionId;


//*************************************************************************
//*****************	class EDROOMCommSAP	***************************************

//*************************************************************************


class EDROOMCommSAP{
private:

	static unsigned long idInterface;

public:

#ifdef _EDROOM_IS_DEBUG_

	int registerInterface( EDROOMInterface &, EDROOMActor*, char * InterfaceName );


#else
	int registerInterface( EDROOMInterface &, EDROOMActor* );

#endif

#ifdef _EDROOM_VERSION2002 

	int registerInterface( EDROOMInterface & inte, EDROOMActor* act, int a ){
		return registerInterface(inte,act);
	}

#endif

	
	
	ConnectionId connect(EDROOMActor *,EDROOMInterface &,EDROOMInterface &,
						EDROOMSignal  (f1) (EDROOMSignal ), EDROOMSignal  (f2) (EDROOMSignal ));
	

#ifdef _EDROOM_SYSTEM_CLOSE

	int deregisterInterface(EDROOMInterface &);

	int disconnect(ConnectionId &);

#endif


};

#ifdef _EDROOM_IS_EMBEDDED_

class M_d_R_EDROOMConnection: public M_d_R {


	public:


		void SetMemory(unsigned int elemNumber, EDROOMConnection* pPoolData,unsigned char * pPoolMarks);


		EDROOMConnection * AllocData();

  };


#endif




#endif
 
 
//**************************************************************************
// IMPLEMENTACION DE LOS ACTORES DE UN MODELO EDROOM BASADAS EN LAS
// PRIMITIVAS DE TIEMPO REAL DEFINIDAS EN prim_pp.h

// Constantes
//                                      NUMAXMENDEF
//                                      PRIOACTINIT
//                                      STACKDEF

// Clase base EDROOMActor     probado:

//                                                                              que crea las tareas en EDROOMAC\proy_bor\actor.ide

//**************************************************************************


#ifndef __EDROOM_Actor_H
#define __EDROOM_Actor_H


 
 //************************************************************************

class EDROOMActor{

#ifdef _EDROOM_TASK_POSIX_

    friend void * EDROOMTareaActor(void * param);

#else

    #ifdef _EDROOM_TASK_ONE_PARAM_VOID
    friend void EDROOMTareaActor(void * param);
    #else
    friend void EDROOMTareaActor(void);
    #endif

#endif

	friend class EDROOMCommSAP;
	friend class EDROOMTimingSAP;
	friend class EDROOMConnectionChannel;
	friend class EDROOMQueue;
	friend class EDROOMIRQInterface;
#ifdef _EDROOM_HAND_EXCEP_

	friend class EDROOMExceptionSAP;
#endif




	//*************************************************************************
		//Protected Attributes
	//*************************************************************************


protected:



			// Atributos protegidos de la clase

	Pr_SemaphoreRec mutexSem; // Semaforo para garantizar la exclusion mutua
									// de los atributos de cada Actor

	Pr_Task actorTask;

	Pr_Task *p_task;

								  // Apuntador a un objeto de la clase Pr_Task que
								  // representa la tarea que realiza el comportamiento
								  // del Actor

	M_d_R_EDROOMConnection *p_ConnectPool; //Pointer to the Connection Pool


	EDROOMCommSAP edroomcommSAP; // SAP del servicio de comunicaciones, hay uno en cada
							  // Actor

	EDROOMMessage 		MessageBack;    //MessageBack in a Synchr Comm
	EDROOMSynchrMessage SynchrMsg;    //MessageOut in a Synchr Comm
	EDROOMSynchrMessageManager SynchrMsgManager; //Manager of a a SynchrMessage

	EDROOMQueue msgQueue;  // Cola de mensajes del Actor

	M_d_R_EDROOMMessage *p_MsgPool;     // Apuntador al bloque de memoria que
								// maneja los mensajes.

	int numMaxMens ;                // Numero maximo de mensajes que se desea que esten
								// esten en la cola, no se contabilizan los mensajes
								// URGENTES
								// -1 indica que no hay limite en el numero de mensajes

	unsigned numPrior;      // Numero de prioridades distintas en los mensajes
								// Determinado por los protocolos del interfaz del Actor


	unsigned numConex;   // Numero de conexiones entre los actores componentes
								// de ese actor, esta determinado por el dise� del
								// actor.


#ifndef _EDROOM_NO_INHERIT_MSG

    EDROOMActor *blockActor; //Actor que mantiene bloqueado al actor actual en una comunicaci� s�crona

//*************************************************************************
		//Protected Methods
//*************************************************************************

    void SetMaxPrioSendMsg( EDROOMPriority  priority); //Asegura la herencia de prioridad entre actores
    bool SetMaxPrioInvokeMsg( EDROOMActor * actor, EDROOMPriority  priority); //Asegura la herencia de prioridad entre actores y detecta Interbloqueo

#endif

//*************************************************************************
		//Protected Methods
//*************************************************************************


	virtual void EDROOMBehaviour()=0; // virtual para forzar la vinculacion dinamica



	#ifdef _EDROOM_SYSTEM_CLOSE


	void FreeMem(); 					// para liberar la memoria del actor


	bool finished; // indica que ha terminado de ejecutarse el
									 // comportamiento del acto

	#endif


//*************************************************************************
		//Public Attributes
//*************************************************************************


public:

	#ifdef _EDROOM_IS_DEBUG_

	const char * const EDROOMName;

	const char * const EDROOMNameContainer;

    void EDROOMTraceTrans(EDROOMTransId & TransId, EDROOMSignal signal);

    void EDROOMTraceStateEntry();

	#endif


public:



	EDROOMMessage *Msg;   // Mensaje actual que esta tratando el actor.
							  // Solo se puede tratar un mensaje cada vez
							  // Si se quiere guardar alguna informacion que transporta
							  // el mensaje debera hacerse una copia de esa informacion
							  // puesto que cada vez que se termina de tratar un mensaje
							  // se libera toda su memoria.

	EDROOMMessage *MsgBack;   // Apuntador al Mensaje de retorno de una comunicacion
									// Sincrona

			// metodos publicos de la clase

#ifdef _EDROOM_HAND_EXCEP_

	EDROOMMessage *MsgExc;

	bool NewException;

#endif

	void EDROOMNewMsg();   // Funcion miembro que permite sacar un mensaje
									// de la cola y hace que Msg apunte a ese mensaje
									// a la vez que libera la memoria del anterior
									// mensaje al que estaba apuntando Msg


#ifdef _EDROOM_HAND_EXCEP_

	void EDROOMNewMsgWithExc();   // Funcion miembro que permite sacar un mensaje
									// de la cola y hace que Msg apunte a ese mensaje

#endif



	// CONSTRUCTOR


	#ifdef _EDROOM_IS_DEBUG_

	EDROOMActor(const char * const name,const char * const nameContainer,unsigned numPrior,
																	unsigned numConex, int numMaxMen,
																	EDROOMPriority taskPrio,
																	unsigned stack

	#ifdef _EDROOM_IS_EMBEDDED_

																	,EDROOMActorMemory * pActorMemory

	#endif


																	);


	#else


	EDROOMActor(unsigned numPrior,unsigned numConex, int numMaxMen, EDROOMPriority taskPrio,
																	unsigned stack

	#ifdef _EDROOM_IS_EMBEDDED_

																	,EDROOMActorMemory * pActorMemory

	#endif


																	);


	#endif


	// Funcion miembro Config
	// Virtual pura y por tanto de definicion obligatoria en los Actores
	// En ella esta definida la configuracion de actor
	// En la configuracion se incluye por este orden:
	//                              llamada a la funcion Config de todos los actores componentes.
	//                              conexion de los interfaces de los componentes
	//                      registro de los interfaces propios


	//int  EDROOMConfig()=0;

	// Funcion miembro Start
	// Crea una tarea a partir de la funcion miembro EDROOMBehaviour

	int EDROOMStart  ();


	// FUNCION QUE CHEQUEA SI EL ACTOR ESTA TERMINADO

	#ifdef _EDROOM_SYSTEM_CLOSE

	virtual  bool EDROOMActorTerminado();


// DESTRUCTOR

	virtual ~EDROOMActor();

	#endif

	// FUNCION QUE TERMINA EL ACTOR


	void EDROOMDestroy();


};


#ifdef _EDROOM_IS_EMBEDDED_

class EDROOMActorMemory{


friend class EDROOMActor;
friend class EDROOMQueue;
friend class EDROOMTimingSAP;

	protected:

	M_d_R_EDROOMConnection  ConnectionsMem;
	M_d_R_EDROOMMessage  MessagesMem;
	EDROOMQueue::M_d_R_QueueNode  QueueNodesMem;
	EDROOMQueue::QueueHead        * QueueHeadsMem;

	void SetMemory(unsigned int connectNumber_, EDROOMConnection * ConnectionsMem_, unsigned char * ConnectionsMemMarks,

					unsigned int messagesNumber_,EDROOMMessage  * MessagesMem_, unsigned char * MessagesMemMarks_, EDROOMQueue::QueueNode * QueueNodesMem_, unsigned char * QueueNodesMemMarks_,

					EDROOMQueue::QueueHead *QueueHeadsMem_);

};

#endif
#endif
 
 
#ifndef __EDROOM_ExceptionSAP_H
#define __EDROOM_ExceptionSAP_H

#ifdef _EDROOM_HAND_EXCEP_

class EDROOMExceptionSAP{  //tratamiento de excepciones

	public:

	EDROOMActor  *pActor;

	EDROOMExceptionSAP(EDROOMActor *actor);

	void newException();

};

#endif

#endif
 
 
 
 
#endif
