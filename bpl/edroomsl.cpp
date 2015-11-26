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


// INCLUDES
#include "edroomdf.h"
#include "edroomsl.h"

//*********************************************************
//***********         DEFINES *****************************
//*********************************************************

//This code is affected for this defines



//*******************EDROOM AUTOMATIC TARGET OPTIONS******************
//
//	EDROOM automatically configure this defines in the edroomdf.h file
//
// _EDROOM_IS_EMBEDDED_
// _EDROOM_IS_DEBUG_
// _EDROOM_HAND_EXCEP_
//


//*******************TARGET OPTIONS******************
//
//	You must configure them in the prim_pp.h file, not here
//
// Let you customize your target

//_EDROOM_SYSTEM_CLOSE

//_EDROOM_MainMustWait

//_EDROOM_HANDLE_IRQS




//*******************SCHEDULING*********************
//**
//**	You must configure them in the prim_pp.h file, not here
//**  only active to test some characteristics of the EDROOM VM scheduling
//** ,the performance can decrease if you activate them

//_EDROOM_NO_INHERIT_MSG

//_EDROOM_URGENT_INVOKE

//_EDROOM_INTERNAL_EVENT_RESTORE_PRIO

//_EDROOM_INFORM_AT_NO_NEED_WAIT




//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

//******************    FUNCIONES AMIGAS    ***************************

//                                                                       tar_Actor

// permite crear las tareas que  ejecutan el comportamiento de los actores

#ifdef _EDROOM_TASK_POSIX_

void * EDROOMTareaActor(void * param){

#else

#ifdef _EDROOM_TASK_ONE_PARAM_VOID
void EDROOMTareaActor(void * param){
#else
void EDROOMTareaActor(void){
#endif

#endif

	EDROOMActor* ap_actor;
	Pr_Receive(&ap_actor,sizeof(EDROOMActor*));
	ap_actor->EDROOMBehaviour();

#ifdef _EDROOM_SYSTEM_CLOSE

	ap_actor->FreeMem();

#endif


#ifdef _EDROOM_TASK_POSIX_

    return 0;
#endif

}

///////////////////////////INCLUDES///////////////////////////////



#ifdef _EDROOM_IS_EMBEDDED_

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CLASE QUE REALIZA UN ALMACEN REDIMENSIONABLE PERO NO GESTIONABLE DE FORMA
// DISTRIBUIDA

// class M_d_R


//

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CONSTRUCTOR
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void M_d_R::SetMemory(unsigned int dimen, void * Memory,  unsigned char * MarksMemory, unsigned int size_ ){

			unsigned int n;

			if (dimen==0)
				{
				vacio=1;
				bloque= 0;

				}
			else{
				 vacio=0;

				 bloque=(EDROOMByte *)Memory;
//				 libre =MarksMemory new unsigned char[dimen]; // RESERVA MEM PARA LAS MARCAS

				 libre=MarksMemory;

				 size=size_;

				 for (n=0;n<dimen;n++)
					*(libre + n)=1;  // MARCA TODOS COMO DISPONIBLES

				 elem_dispo=bloque;
				 dimension=dimen;
				 ind_dispo=0;

			}
	 }


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PETICION DE APUNTADOR A UN ELEMENTO DE LA CLASE T QUE ESTE LIBRE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

EDROOMByte * M_d_R::AllocData()  {

		unsigned int ind_elem;


		return AllocData(ind_elem);

}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Devuelve tambien el indice del elemento dentro del array


EDROOMByte * M_d_R::AllocData(unsigned int & ind_elem)
	  {
			EDROOMByte *nuevo;
			unsigned int contador,indice;

			//semaforo.Wait(); // PIDE ACCESO A MEM COMPARTIDA

			if (vacio){
				nuevo=0;
				ind_elem=UINT_MAX;

#ifdef _EDROOM_IS_DEBUG_

				EDROOMFError << " POOL Empty";
				EDROOMFError.close();
				exit(1);

#endif

				}
			else
				{


				nuevo=elem_dispo;  // DEVUELVE EL APUNTADOR
				*(libre+ind_dispo)=0; // MARCA EL ELEMENTO
				ind_elem=ind_dispo;

				// MIRA SI ESTA DISPONIBLE OTRO ELEMENTO,
				// Y SI NO QUEDA EL BLOQUE MARCADO COMO VACIO

				vacio=1;
				contador=0;
				elem_dispo=0;
				do
					{
					indice=(ind_dispo + 1 +contador)%dimension;   //indice donde busca

					if(*(libre+indice))
						{
						//ENCUENTRA UNO LIBRE

						elem_dispo=bloque+(indice*size);
						vacio=0;
						ind_dispo=indice;
						}
					contador++;
					}
				while((contador<dimension)&&(vacio));


				} // fin del else
			//semaforo.Signal();
			return(nuevo);
		 }


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PERMITE DEVOLVER UN ELEMENTO AL BLOQUE PARA QUE SE PUEDA DISPONER OTRA VEZ
// DE EL. RETORNA UN 1 SI EL ELEMENTO YA ESTABA DISPONIBLE DE ANTES Y POR TANTO
// HA HABIDO UN ERROR EN EL MANEJO DEL BLOQUE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Pasando un puntero a void

int M_d_R::FreeData(void * ap)
		{
		  int error;
		  EDROOMByte* apunt;

		  long posicion,dimensionLong;
		  unsigned int indice;

		  apunt=(EDROOMByte*)ap;

		  posicion = (apunt-bloque)/size;
		  dimensionLong=dimension;

		  //semaforo.Wait();  // PIDE ACCESO A LA MEMORIA COMPARTIDA

			if ((posicion <0)||(posicion >= dimensionLong)){

						error=1;

#ifdef _EDROOM_IS_DEBUG_

						EDROOMFError <<"POOL Free Error";
						EDROOMFError.close();
						exit(1);

#endif

			}
			else
			{


				indice=(unsigned int) posicion; //esta chequeado que es mayor que cero
													// y menor que dimension

				*(libre+indice)=1; // LO MARCA COMO DISPONIBLE
				if (vacio)            // SI ESTABA VACIO EL BLOQUE
									// LO COLOCA COMO A DISPONER
					elem_dispo=apunt;

				ap=NULL; // El puntero sale apuntando a NULL

				error=0;
				vacio=0;

			}
		  //semaforo.Signal();
		  return (error);
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CLASE QUE REALIZA UN ALMACEN REDIMENSIONABLE Y GESTIONABLE DE FORMA
// DISTRIBUIDA

// template <class T > class Mem_din_R_D


//							PROBADA EN manj_mem\proy_bor\manj_mem.ide
							// falta probar mejoras


// El tipo T puede ser cualquiera que tenga definido constructor y destructor


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CONSTRUCTOR
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 M_d_R_D::M_d_R_D(unsigned int dimen, void * Memory,  unsigned char * MarksMemory, unsigned int size_)

 #ifdef _EDROOM_IS_DEBUG_

	:semaforo("Semaforo Bloque")

 #endif

 {
	SetMemory(dimen, Memory,  MarksMemory, size_) ;
 }


 void M_d_R_D::SetMemory(unsigned int dimen, void * Memory,  unsigned char * MarksMemory, unsigned int size_)

 {

			M_d_R::SetMemory(dimen, Memory,  MarksMemory, size_);


 }

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PETICION DE APUNTADOR A UN ELEMENTO DE LA CLASE T QUE ESTE LIBRE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
EDROOMByte * M_d_R_D::AllocData(void)
	  {

		unsigned int ind_elem;


		return AllocData(ind_elem);


}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Devuelve tambien el indice del elemento dentro del array


EDROOMByte * M_d_R_D::AllocData(unsigned int & ind_elem)
	  {


			EDROOMByte *pData;

			semaforo.Wait(); // PIDE ACCESO A MEM COMPARTIDA

			pData=M_d_R::AllocData(ind_elem);

			semaforo.Signal(); // PIDE ACCESO A MEM COMPARTIDA

			return(pData);
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PERMITE DEVOLVER UN ELEMENTO AL BLOQUE PARA QUE SE PUEDA DISPONER OTRA VEZ
// DE EL. RETORNA UN 1 SI EL ELEMENTO YA ESTABA DISPONIBLE DE ANTES Y POR TANTO
// HA HABIDO UN ERROR EN EL MANEJO DEL BLOQUE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Pasando un puntero a void

int M_d_R_D::FreeData(void * ap)
		{

		  int error;
		  semaforo.Wait();  // PIDE ACCESO A LA MEMORIA COMPARTIDA

		  error = M_d_R::FreeData(ap);

		  semaforo.Signal();

		  return error;

}


#endif



//*************************************************************************
//******************FUNCIONES MIEMBRO PUBLICAS DE EDROOMMessage *************


//*************************************************************************
//CONSTRUCTOR

	EDROOMMessage::EDROOMMessage(){

		pPoolMess=0; // Por defecto no hay ningun manejador de memoria
									// Para el mensaje.
		pPoolData=0; // Por defecto no hay ningun manejador de memoria
									// Para el dato.
	}

//*************************************************************************
//cancel

	bool EDROOMMessage::cancelled(){return 0;} // No se cancelan los mensajes corrientes
											  // Solo los de Timeout


//*************************************************************************
//EDROOMInterface


EDROOMInterface   * EDROOMMessage::EDROOMinterface (){return(edroominterface);} //interface component on wich message arrived


//*************************************************************************
//reply

void EDROOMMessage::reply(EDROOMSignal signal){

	// Lo mando con la misma prioridad que
	 
	edroominterface->send(signal,priority);
}
//**************************************************************************

void EDROOMMessage::reply(EDROOMSignal signal, void *dataObjetPtr
																, M_d *apManMemDat){

	//Lo mando con la misma prioridad que el mensaje enviado

	edroominterface->send(signal,priority,dataObjetPtr,apManMemDat);
}

//**************************************************************************

void EDROOMMessage::reply(EDROOMMessage *Msg){

	//Lo mando con la misma prioridad que el mensaje enviado

	edroominterface->send(Msg->signal,Msg->priority,Msg->data,Msg->pPoolData);
}

//**************************************************************************

void EDROOMMessage::reply(EDROOMSignal signal, EDROOMMessage *Msg){

	//Lo mando con la misma prioridad que el mensaje enviado

	edroominterface->send(signal,priority,Msg->data,Msg->pPoolData);

	Msg->data=NULL;
}

//*************************************************************************
//******************FUNCIONES AMIGAS DE EDROOMMessage ***********************

//*************************************************************************
//EDROOMFreeMessage

void EDROOMFreeMessage(EDROOMMessage *&apMsg){
					//Compruebo que el apuntador es distinto de NULL

	if (apMsg){

		if (apMsg->data){       //Si hay algun dato en el mensaje
			if(apMsg->pPoolData)
				apMsg->pPoolData->FreeData(apMsg->data); //

#ifndef _EDROOM_IS_EMBEDDED_
			else
				delete(apMsg->data);
#endif
		}
		//Si hay manejador de memoria para el mensaje se utiliza

		void *auxApMsg=apMsg;

		if(apMsg->pPoolMess)
			apMsg->pPoolMess->FreeData(auxApMsg); //apuntador al manejador de memoria de los Mensajes

#ifndef _EDROOM_IS_EMBEDDED_
		else
				// Si no se libera utilizando delete
			delete(apMsg);
#endif

		apMsg=0; //Ya se ha liberado la memoria


	}
}

void EDROOMFreeMsgBack(EDROOMMessage *&apMsg){
					//Compruebo que el apuntador es distinto de NULL

	if (apMsg){

		if (apMsg->data){       //Si hay algun dato en el mensaje
			if(apMsg->pPoolData)
				apMsg->pPoolData->FreeData(apMsg->data); //

#ifndef _EDROOM_IS_EMBEDDED_
			else {
				delete(apMsg->data);
			}
#endif

		}
		apMsg=0; //Ya se ha liberado la memoria

	}
}



//*************************************************************************
//******************FUNCIONES MIEMBRO PUBLICAS DE EDROOMSynchrMessage ******


//*************************************************************************
//CONSTRUCTOR


#ifdef _EDROOM_IS_DEBUG_

EDROOMSynchrMessage::EDROOMSynchrMessage():semSynchr("Sem Sincr",0){
		p_MsgBack=0;
}
#else
EDROOMSynchrMessage::EDROOMSynchrMessage():semSynchr(0){
		p_MsgBack=NULL;
}

#endif



//*************************************************************************
//reply()



void EDROOMSynchrMessage::reply(EDROOMSignal signal){

	EDROOMSynchrMessage::reply(signal,NULL,0);

}

//**************************************************************************

void EDROOMSynchrMessage::reply(EDROOMSignal signal, void *dataObjetPtr
																, M_d *apManMemDat){

	if(waiting){

		p_MsgBack->signal=edroominterface->applyConverFunct(signal); // Completo el mensaje
		p_MsgBack->data=dataObjetPtr;
		p_MsgBack->pPoolData=apManMemDat;

		waiting=0; // No debe esperar mas
		semSynchr.Signal(); //Señala al emisor para que prosiga
	}

}
//**************************************************************************

void EDROOMSynchrMessage::reply(EDROOMSignal signal, EDROOMMessage *Msg){

	EDROOMSynchrMessage::reply(signal,Msg->data,Msg->pPoolData);

	Msg->data=NULL;
}

//**************************************************************************

void EDROOMSynchrMessage::reply(EDROOMMessage *Msg){

	EDROOMSynchrMessage::reply(Msg->signal,Msg->data,Msg->pPoolData);

	Msg->data=NULL;
}

//**************************************************************************


//*************************************************************************
//******************FUNCIONES MIEMBRO PRIVADAS DE EDROOMTimeOutMessage ******

//*************************************************************************
//cancelled()

bool EDROOMTimeOutMessage::cancelled(){
	bool cancl;

	pMutexSem->Wait(); //Capturo la exclusion mutua

	if(pTimerInf){

		if(pTimerInf->state==edroomTimerCancelled)
			 cancl=1;
		else
			cancl=0;

		void * aux_void_pTimerInf= pTimerInf;
	

		pPoolTimerInf->FreeData(aux_void_pTimerInf);  // Devuelve la memoria

		pTimerInf=0;		

	}
	else
		cancl=1; // Si no hay puntero al status ha habido un error
					// consideramos que el mensaje esta cancelado

	pMutexSem->Signal(); //Libero la exclusion mutua
	return(cancl);
}

//*************************************************************************



//*******************************************************************************
//******************FUNCIONES MIEMBRO PUBLICAS DE EDROOMSynchrMessageManager ******


// Constructor


#ifdef _EDROOM_IS_DEBUG_

EDROOMSynchrMessageManager::EDROOMSynchrMessageManager(EDROOMSynchrMessage *  SynchrMsg__):semMutex("Sem Mutex",1){


#else

EDROOMSynchrMessageManager::EDROOMSynchrMessageManager(EDROOMSynchrMessage *  SynchrMsg__):semMutex(1){


#endif

		SynchrMsg=SynchrMsg__;

}



//	 AllocData


EDROOMSynchrMessage *  EDROOMSynchrMessageManager::AllocData(void){

		semMutex.Wait();
		return SynchrMsg; //Semaforo para la sincronizacion del mensaje

	}

#ifdef _EDROOM_IS_EMBEDDED_

int EDROOMSynchrMessageManager::FreeData(void * apunt){

				semMutex.Signal();
				return 0;
};

#else
int EDROOMSynchrMessageManager::FreeData(void * &apunt){

				semMutex.Signal();
				return 0;
};

#endif




#ifdef _EDROOM_IS_EMBEDDED_


		
void M_d_R_EDROOMMessage::SetMemory(unsigned int elemNumber, EDROOMMessage * pPoolData,unsigned char * pPoolMarks){


	M_d_R::SetMemory(elemNumber,pPoolData, pPoolMarks, sizeof (EDROOMMessage));


}


EDROOMMessage * M_d_R_EDROOMMessage::AllocData(){

				return (EDROOMMessage * )M_d_R::AllocData();


}

	
void M_d_R_EDROOMTimeOutMessage::SetMemory(unsigned int elemNumber, EDROOMTimeOutMessage* pPoolData,unsigned char * pPoolMarks){

	M_d_R::SetMemory(elemNumber,pPoolData, pPoolMarks, sizeof (EDROOMTimeOutMessage));

}



	   
EDROOMTimeOutMessage * M_d_R_EDROOMTimeOutMessage::AllocData(){

				return (EDROOMTimeOutMessage * )M_d_R::AllocData();
}


#endif 



//*****************************************************************************************
//********************************** DENINICION DE LAS FUNCIONES MIEMBRO DE ***************
//********************************************EDROOMInterface ********************************

#ifdef _EDROOM_IS_DEBUG_

const char * EDROOMErrorCommNames[8] =  { "EDROOMNoErrorComm", "EDROOMEmptyPoolMens", "EDROOMSignalInvalidate", "EDROOMFullQueueMens",
							"EDROOMInterfaceDisconnected", "EDROOMCommTimeOut", "EDROOMMsgBackUsed", "EDROOMInterblock" };

#endif

//*****************************************************************************************
//************************************CONSTRUCTOR ********************************

#ifdef _EDROOM_IS_DEBUG_

EDROOMInterface::EDROOMInterface(){

		registered=0;  //No esta registrado
		connected=0; // No esta connected

}
#endif

//*****************************************************************************************
//************************************EDROOMInterface::send ********************************



	// VERSION 2.
	// Revisada y comentada
#ifdef _EDROOM_IS_DEBUG_

EDROOMErrorComm EDROOMInterface::send (EDROOMSignal signal, EDROOMPriority priority){

	EDROOMErrorComm error=EDROOMNoErrorComm;

	// Ver si hay errores
	// Si hay errores el mensaje no se manda

	if (!connected){

		error= EDROOMInterfaceDisconnected;

	}else   p_channel->send(signal,priority,NULL,0);

	if (error!=EDROOMNoErrorComm){
		EDROOMFError <<name << " " << EDROOMErrorCommNames[error];
		EDROOMFError.close();
		exit(1);
	}

	return error;

}

#else

void EDROOMInterface::send (EDROOMSignal signal, EDROOMPriority priority){

	p_channel->send(signal,priority,NULL,NULL);

}

#endif



	// VERSION 3.
	// Revisada y comentada

#ifdef _EDROOM_IS_DEBUG_

EDROOMErrorComm EDROOMInterface::send (EDROOMSignal signal, EDROOMPriority priority, void * dataObjetPtr,M_d * apMemDats){

	EDROOMErrorComm error=EDROOMNoErrorComm;

	// Ver si hay errores
	// Si hay errores el mensaje no se manda

	if (!connected){

		error= EDROOMInterfaceDisconnected;

	}else
		p_channel->send(signal,priority,dataObjetPtr,apMemDats);

	if (error!=EDROOMNoErrorComm){
		EDROOMFError <<name << " " <<EDROOMErrorCommNames[error];
		EDROOMFError.close();
		exit(1);
	}

	return error;


}
#else

void EDROOMInterface::send (EDROOMSignal signal, EDROOMPriority priority, void * dataObjetPtr,M_d * apMemDats){

		p_channel->send(signal,priority,dataObjetPtr,apMemDats);

}

#endif





	// VERSION 4.
	// Revisada y comentada

#ifdef _EDROOM_IS_DEBUG_

EDROOMErrorComm	EDROOMInterface::send (EDROOMSignal signal,EDROOMPriority priority,EDROOMMessage *message){  // reenvio de un mensaje

	EDROOMErrorComm error=EDROOMNoErrorComm;

	// Ver si hay errores
	// Si hay errores el mensaje no se manda

	if (!connected){

		error= EDROOMInterfaceDisconnected;

	}else {
		p_channel->send(signal,priority,message->data,message->pPoolData);
		message->data=NULL;
	}

	if (error!=EDROOMNoErrorComm){
		EDROOMFError <<name << " " <<EDROOMErrorCommNames[error];
		EDROOMFError.close();
		exit(1);
	}

	return error;


}

EDROOMErrorComm	EDROOMInterface::send(EDROOMSignal signal,EDROOMMessage *message){  // reenvio de un mensaje

		return send(signal,message->priority,message);

}

EDROOMErrorComm	EDROOMInterface::send(EDROOMMessage *message){  // reenvio de un mensaje

		return send(message->signal,message->priority,message);

}

#else

void	EDROOMInterface::send(EDROOMSignal signal,EDROOMPriority priority,EDROOMMessage *message){  // reenvio de un mensaje

	p_channel->send(signal,priority,message->data,message->pPoolData);
	message->data=NULL;

}

void	EDROOMInterface::send(EDROOMSignal signal,EDROOMMessage *message){  // reenvio de un mensaje

	send(signal,message->priority,message);

}

void	EDROOMInterface::send(EDROOMMessage *message){  // reenvio de un mensaje

	send(message->signal,message->priority,message);

}


#endif


//*****************************************************************************************
//************************************EDROOMInterface::invoke ********************************




EDROOMMessage * EDROOMInterface::invoke(EDROOMSignal signal,EDROOMPriority priority){

	return invoke(signal,priority, NULL, 0);
}




//*****************************************************************************************
//************************************EDROOMInterface::invoke ********************************


EDROOMMessage * EDROOMInterface::invoke(EDROOMSignal signal, void *dataObjetPtr, M_d * apMemDats){

	return invoke(signal, EDROOMprioURGENT, dataObjetPtr,apMemDats);

}



EDROOMMessage * EDROOMInterface::invoke(EDROOMSignal signal, EDROOMPriority priority, void *dataObjetPtr, M_d * apMemDats){


#ifdef _EDROOM_IS_DEBUG_

	EDROOMErrorComm error=EDROOMNoErrorComm;

	// Ver si hay errores
	// Si hay errores el mensaje no se manda

	if (!connected){

		error= EDROOMInterfaceDisconnected;

    	EDROOMFError <<name << " " <<EDROOMErrorCommNames[error];
		EDROOMFError.close();
		exit(1);
	}

#endif

		return p_channel->invoke(signal,priority,dataObjetPtr, apMemDats);



}


//*****************************************************************************************
//************************************EDROOMInterface::invokeForward ********************************


EDROOMMessage * EDROOMInterface::invoke(EDROOMSignal signal, EDROOMPriority priority, EDROOMMessage *message){

		EDROOMMessage * msgBack;
		msgBack=invoke(signal,priority,message->data,message->pPoolData);
		message->data=NULL;

		return (msgBack);

}

EDROOMMessage * EDROOMInterface::invoke(EDROOMSignal signal, EDROOMMessage *message){

	return invoke(signal,message->priority,message);

}

EDROOMMessage * EDROOMInterface::invoke(EDROOMMessage *message){

	  return invoke(message->signal,message->priority,message);
}






EDROOMSignal EDROOMInterface::applyConverFunct(EDROOMSignal signal_){


	return p_channel->converFunct(signal_);

}




//identity test functions:

int EDROOMInterface::operator ==(EDROOMInterface &edroominterface){
		return (idInterface==edroominterface.idInterface);
	}

int EDROOMInterface::operator !=(EDROOMInterface &edroominterface){
		return (idInterface!=edroominterface.idInterface);
	}



#ifdef _EDROOM_HANDLE_IRQS

//**************************************************************************
//********* EDROOMInterfaceIRQ ***********************************************

void EDROOMIRQInterface::InstallHanler(){
		if(!IRQInstalled) {
			Pr_IRQManager::Install(HandIRQ,IRQ);
			Pr_IRQManager::Enable(IRQ);
			IRQInstalled=1;
		}

}

void EDROOMIRQInterface::DeInstallHanler(){
		if(IRQInstalled) {
			Pr_IRQManager::Disable(IRQ);
			Pr_IRQManager::Install(HandIdle,IRQ);
			Pr_IRQManager::Enable(IRQ);
			IRQInstalled=0;

		}
}

void EDROOMIRQInterface::EndIRQInterm(){

							Pr_IRQManager::Disable(IRQ);
							TerminarIRQLook->Signal();
							IRQEventLook->Signal();


}

EDROOMIRQInterface::EDROOMIRQInterface(	Pr_IRQHandler HandIRQ_,	Pr_IRQHandler HandIdle_,	Pr_IRQEvent &IRQEventLook_ , Pr_SemaphoreBin & TerminarIRQLook_,	EDROOMPriority prio, int IRQ_){

						HandIRQ= HandIRQ_;
						HandIdle =HandIdle_;
						TerminarIRQLook= &TerminarIRQLook_;
						IRQEventLook= &IRQEventLook_;
						priority=prio;
						IRQInstalled=0;
						IRQ=IRQ_;
}


void	EDROOMIRQInterface::NewIRQMsg(EDROOMSignal signal, void * dataObjetPtr, M_d * pDataPool){

		EDROOMMessage *apMsg;


								// Apuntador a un objeto de la clase Pr_Task que
								  // representa la tarea que realiza el comportamiento
								  // del Actor

		p_actor->mutexSem.Wait();


#ifdef _EDROOM_NO_INHERIT_MSG

		p_actor->actorTask.SetMaxPrioMsg( priority);

#else

		p_actor->SetMaxPrioSendMsg(priority);

#endif

		// Reservar memoria para el mensaje

		apMsg=p_actor->p_MsgPool->AllocData();

		// Rellenar sus campos;

		// Señal

		apMsg->signal=signal;

		//apuntador al manejador de memoria de los Mensajes del Receptor
		 //apuntador al manejador de memoria de los Mensajes

		apMsg->pPoolMess=p_actor->p_MsgPool;

		//dato enviado
		apMsg->data=dataObjetPtr;

		//manejador de memoria
		apMsg->pPoolData=pDataPool;

		//Prioridad del mensaje

		apMsg->priority=priority;

		//Completo el campo interf para que apunte al puerto por el que se recibe
		// el mensaje
		apMsg->edroominterface=this;

		// Se mete el mensaje en la cola asociada a este puerto.

		p_actor->msgQueue.In(apMsg);


		p_actor->mutexSem.Signal();

	}

#endif



//*********************************************************************
//*********************************************************************
// class EDROOMTimer


EDROOMTimer::EDROOMTimer(){indice=UINT_MAX;}  // Se inicializa el indice a UINT_MA

bool EDROOMTimer::correcto(){return (indice < UINT_MAX);}


//*********************************************************************
//*********************************************************************
// class EDROOMTimingSAP


//**********************************************************************
// CONSTRUCTOR

#ifndef _EDROOM_IS_EMBEDDED_

EDROOMTimingSAP::EDROOMTimingSAP(EDROOMActor * p_Actor_,unsigned int Num_max_timers):

#else

EDROOMTimingSAP::EDROOMTimingSAP(EDROOMActor * p_Actor_,unsigned int Num_max_timers,EDROOMTimmingMemory *pTimmingMemory):

#endif



#ifdef _EDROOM_IS_DEBUG_

			mutexSem("Timming Mutex"),

			synchrSem("TimmingSemWaitTimed",0),

			timerTask(EDROOMfunTimer,  "Tarea_timer", EDROOMprioURGENT, timer_Stack)

			,waitActorSem("TimmingSemWaitEnd",0)


#else
			timerTask(EDROOMfunTimer, EDROOMprioURGENT, timer_Stack),
			synchrSem(0)

			,waitActorSem(0)

#endif


{


#ifdef _EDROOM_IS_DEBUG_

		if (Num_max_timers>=UINT_MAX)  // El limite es UINT_MAX-1
				Num_max_timers=UINT_MAX-1;

#endif

			ptimerTask=&timerTask;

			pActor=p_Actor_; //Apuntador al actor propietario de la clase

			// Semaforo para asegurar la exclusion mutua


#ifdef _EDROOM_SYSTEM_CLOSE

			SAPFinished=0;

#endif



#ifndef _EDROOM_IS_EMBEDDED_

			// reservo memoria para manejar dinámicamente los estados
			//de los timer

		  timerInfPool = new M_d_R_EDROOMTimerInf (Num_max_timers);
			// reservo memoria para manejar dinámicamente los mensajes

		  timeOutMsgPool = new M_d_R_EDROOMTimeOutMessage (Num_max_timers);



#else

		  timerInfPool=&pTimmingMemory->TimerInfMem;

		  timeOutMsgPool=&pTimmingMemory->TimeOutMessMem;


#endif


		  nextTimer=0;



	}





//**********************************************************************
// DESTRUCTOR

#ifdef _EDROOM_SYSTEM_CLOSE

EDROOMTimingSAP::~EDROOMTimingSAP(){

		mutexSem.Wait();

		SAPFinished=true;

		if(nextTimer){

			nextTimer->state=edroomTimerCancelled;

			EDROOMTimerInf *aux=nextTimer->next;

         pActor->mutexSem.Wait();

			while (aux){

				EDROOMMessage * auxMsg= (EDROOMMessage *)aux->Msg;

				EDROOMFreeMessage(auxMsg);

				aux->Msg=NULL;

				EDROOMTimerInf * aux2=aux;

				aux=aux->next;

				timerInfPool->FreeData(aux2);

			}

         pActor->mutexSem.Signal();

			mutexSem.Signal();

			synchrSem.Signal();

		}

		else{

				//Mandar un Timer dummy


				EDROOMTimerConfig timerConfig;

				unsigned int indice;

				timerConfig.timerInf=timerInfPool->AllocData(indice);

				timerConfig.time=0;

				timerConfig.timerInf->state=edroomTimerDummy;

				nextTimer=NULL;

				mutexSem.Signal();

				Pr_Send(timerTask,&timerConfig); // manda el dato al timer


		}

		waitActorSem.Wait();


#ifndef _EDROOM_IS_EMBEDDED_

		delete(timeOutMsgPool);
		delete(timerInfPool);

#endif

	}


#endif



//**********************************************************************
// Función Amiga funEDROOMTimer que implementa el timer

#ifdef _EDROOM_TASK_POSIX_

    void * EDROOMTimingSAP::EDROOMfunTimer(void * paramDumy){

    #else

    #ifdef _EDROOM_TASK_ONE_PARAM_VOID

    void EDROOMTimingSAP::EDROOMfunTimer(void * paramDumy){

    #else

    void EDROOMTimingSAP::EDROOMfunTimer(void){

    #endif

#endif

    EDROOMActor *pActor;

    EDROOMTimingSAP *ptimerSAP;

	EDROOMTimerInf timerInf;
	EDROOMTimerConfig timerConfig;

	Pr_Time currentTime;
	Pr_Time nextTime;

	bool receiveTimerInf;

	#ifdef _EDROOM_SYSTEM_CLOSE

	bool isTimerSAPFinished;

	#endif

	receiveTimerInf=true;


	Pr_Receive(&ptimerSAP,sizeof(EDROOMTimingSAP *)); // Espera a recibir el dato.

   pActor=ptimerSAP->pActor;


	//ptimerSAP->mutexSem.Wait();

	//ptimerSAP->ptimerTask->SetPriority(EDROOMprioMINIMUM);

	//ptimerSAP->mutexSem.Signal();


	do{



		if(receiveTimerInf){
			Pr_Receive(&timerConfig,sizeof(EDROOMTimerConfig)); // Espera a recibir el dato.

			#ifdef _EDROOM_SYSTEM_CLOSE
         	if (timerConfig.timerInf->state!=edroomTimerDummy)
   		#endif
   				#ifdef _EDROOM_INFORM_AT_NO_NEED_WAIT
							;
					#else
   						ptimerSAP->waitActorSem.Signal();
  					#endif


      }

		receiveTimerInf=1;


		//SUPRIMIDO, EL EMISOR SIEMPRE DEJA EN URGENTE ESTA TAREA
				//Se coloca a prioridad maxima para evitar ERROR EN EL SERVICIO DEBIDO A UN SWITCH
				//entre tareas

				//ptimerSAP->mutexSem.Wait();

				//ptimerSAP->ptimerTask->SetMaxPrioTmp(EDROOMprioURGENT);

				//ptimerSAP->mutexSem.Signal();


		// *****************************************************************************************
		// Siempre se ejecuta en prioridad URGENTE por lo que no hay problemas de switch en el siguiente codigo


		currentTime.GetTime();

		// Ver si es necesario esperar

		if (timerConfig.time > currentTime){

			nextTime=timerConfig.time;
			nextTime-=currentTime;

			ptimerSAP->synchrSem.WaitTimed(nextTime);

			currentTime.GetTime();

		}


		ptimerSAP->mutexSem.Wait();                             // captura la exclusión mutua.


		// SE asegura que el semaforo no queda señalado ya que siempre se señala en exclusion mutua

		ptimerSAP->synchrSem.WaitCond();

		#ifdef _EDROOM_SYSTEM_CLOSE

      isTimerSAPFinished=ptimerSAP->SAPFinished;

		if(!isTimerSAPFinished){

		#endif

			// Mira si ha sido cancelado o postpuesto

			if (timerConfig.timerInf->state==edroomTimerActive){

				//Next Timer
				ptimerSAP->nextTimer=timerConfig.timerInf->next;

			  // Si no lo ha sido, manda el mensaje de timeout.

				timerConfig.timerInf->state=edroomTimerSent;

				pActor->mutexSem.Wait();

            // Cambiar la prioridad de la tarea destino si es menor(más prioritaria)


      #ifdef _EDROOM_NO_INHERIT_MSG

		      pActor->actorTask.SetMaxPrioMsg( timerConfig.timerInf->Msg->priority);
      #else

		      pActor->SetMaxPrioSendMsg(timerConfig.timerInf->Msg->priority);
      #endif



		#ifdef _EDROOM_SYSTEM_CLOSE

				if (!pActor->finished)
		#endif
  					pActor->msgQueue.In(timerConfig.timerInf->Msg);


				pActor->mutexSem.Signal();


			}else if (timerConfig.timerInf->state==edroomTimerCancelled){

            //Take the message
            EDROOMMessage * auxMsg= (EDROOMMessage *)timerConfig.timerInf->Msg;

            //Handle next Timer

            ptimerSAP->nextTimer=timerConfig.timerInf->next;

            //Free current Timer
				timerConfig.timerInf->Msg=0;

				ptimerSAP->timerInfPool->FreeData(timerConfig.timerInf);


            //Take actor mutex
				pActor->mutexSem.Wait();

				EDROOMFreeMessage(auxMsg);

				pActor->mutexSem.Signal();

            //EDROOM_NO_SWITCH_IN_TIMER_CANCEL
   						//ptimerSAP->waitActorSem.Signal();


			}

			// Cannot be more Cancelled Timers,


			while(receiveTimerInf && ptimerSAP->nextTimer){

				currentTime.GetTime();

				timerConfig.timerInf=ptimerSAP->nextTimer;

				timerConfig.time=timerConfig.timerInf->time; // DEben ser iguales

				if (timerConfig.time > currentTime){

					receiveTimerInf=0;

					timerConfig.timerInf->state=edroomTimerActive; // Ya esta activo


				} else {

					timerConfig.timerInf->state=edroomTimerSent;


					pActor->mutexSem.Wait();

               #ifdef _EDROOM_NO_INHERIT_MSG

		      	pActor->actorTask.SetMaxPrioMsg( timerConfig.timerInf->Msg->priority);
      			#else

		      	pActor->SetMaxPrioSendMsg(timerConfig.timerInf->Msg->priority);
      			#endif

		#ifdef _EDROOM_SYSTEM_CLOSE

					if (!pActor->finished)
		#endif
						pActor->msgQueue.In(timerConfig.timerInf->Msg);

					pActor->mutexSem.Signal();

				//Next Timer
					ptimerSAP->nextTimer=timerConfig.timerInf->next;


				}

			}


#ifdef _EDROOM_SYSTEM_CLOSE
		}

#endif

     //SUPRIMIDO, SIEMPRE A URGENTE
 	  //	if(receiveTimerInf)
			// Pongo la prioridad de la tarea al minimo si no tiene timerInf
 	  //		ptimerSAP->ptimerTask->SetPriority(EDROOMprioMINIMUM);

     	ptimerSAP->mutexSem.Signal();                             // libera la exclusión mutua.


#ifdef _EDROOM_SYSTEM_CLOSE

	}while(!isTimerSAPFinished);


	ptimerSAP->waitActorSem.Signal();

#else

	}while(1);

#endif

#ifdef _EDROOM_TASK_POSIX_

    return 0;

#endif

}

//**********************************************************************
//**************** FUNCIONES MIEMBRO PUBLICAS **************************

//**********************************************************************
//funcion miembro       InformAt


EDROOMTimer       EDROOMTimingSAP::InformAt(Pr_Time timeValue, EDROOMPriority MsgPrio

																	, void *data
																	,M_d * bloqueMemData){


	 unsigned int indice;
	 EDROOMTimerInf *pTimerInf;
	 EDROOMTimeOutMessage *pTimeOutMsg;
	 EDROOMTimer TimerId;
	 //EDROOMPriority prioActor;
	 //EDROOMPriority maxPrio;

#ifdef _EDROOM_IS_DEBUG_

	 if (registered){ 	// Indica si el puerto esta registrado o no.


#endif


//Prueba


		//Mutex

		pActor->mutexSem.Wait();
		//Hago que el Actor tome momentaneamente la prioridad del mensaje si esta
		//es menor (mas prioritario)

      pActor->p_task->SetMaxPrioTmp(MsgPrio);


      //Reservo el mensaje de TimeOut
      pTimeOutMsg=timeOutMsgPool->AllocData();

		//FinMutex
		pActor->mutexSem.Signal();



		// Saca del bloque un nuevo elemento para guardar el estado del
		// timer

		mutexSem.Wait();  //Mutex del servicio del Temp


		pTimerInf=timerInfPool->AllocData(indice);

		// Si el indice es UINT_MAX ya estan utilizados todos los timers disponibles
		// y el Timer no sera reservado

#ifdef _EDROOM_IS_DEBUG_

		if (indice ==UINT_MAX){
			timerInfPool->FreeData(pTimerInf);
         timeOutMsgPool->FreeData(pTimeOutMsg);
			//Libero Mutex
			mutexSem.Signal();
		}

		else{

#endif

			//Libero Mutex
			mutexSem.Signal();

			Pr_Time currentTime;

			currentTime.GetTime();


			// ******COMPLETO TimerInf

			// El timer va a estar en principio activo
			pTimerInf->state=edroomTimerActive;

			// El tiempo en el que esta programado es timeValue
			pTimerInf->time=timeValue;

			// identificado por el propio indice que es lo que se devuelve

			pTimerInf->indice=indice;

			// Tambien se identifica con el tiempo de programacion del timer
			pTimerInf->timeId=currentTime;


			// *******COMPLETO TimerId

			// Completo el identificador del Timer con el tiempo actual
			// El indice se completa al final

			TimerId.timeId=currentTime;

			// Que el identificador del Timer apunte al bloque con la informacion
			// sobre el timer

			TimerId.timerInf=pTimerInf;

        // *******COMPLETO TimerOutMsg

			// Relleno sus campos

			pTimeOutMsg->signal=0; // Timeout tiene como valor un 0

			pTimeOutMsg->time=timeValue;     //Tiempo en el que se envia el Msg

			pTimeOutMsg->priority=MsgPrio;

			pTimeOutMsg->data=data;

			pTimeOutMsg->pPoolMess=timeOutMsgPool;

			pTimeOutMsg->pPoolData=bloqueMemData;  // Es NULL si no se proporciona

			pTimeOutMsg->edroominterface=this;

			pTimeOutMsg->pTimerInf=pTimerInf;   // apuntador al estado
															//del timer.

			pTimeOutMsg->pPoolTimerInf=timerInfPool; //apuntador al bloque
													// que reserva el estado del timer

			pTimeOutMsg->pMutexSem=&mutexSem;   // apuntador al Semaforo


			// *******COMPLETO TimerInf 2

			pTimerInf->Msg=pTimeOutMsg;


			// *******CAPTURO MUTEX



			mutexSem.Wait();

			if(nextTimer){

				//SUPRIMIDO, EN ESTE CASO SIEMPRE ESTA EN URGENTE // Ajusto prioridad de la tarea
								//ptimerTask->SetPriority(maxPrio);


				// Si el timer esta Activo y tiene mayor tiempo que el nuevo, posponerlo
				// pasadolo otra vez a Ready y dejar este en Active


				if ((nextTimer->state==edroomTimerActive)&& (nextTimer->time >timeValue)){

						nextTimer->state= edroomTimerReady;

						// **** INSERTA EL PRIMERO EN LA COLA

						pTimerInf->next=nextTimer;
						nextTimer=pTimerInf;

						// *******LIBERO MUTEX

						// *******SIGNAL DEL SEMWAITTIMED PARA QUE SE PROGRAME EL SIGUIENTE

						synchrSem.Signal();

						mutexSem.Signal();



				}else {

						bool found;

						found=false;


						// *******YA NO ESTA ACTIVO SOLO LISTO

						pTimerInf->state=edroomTimerReady;


						EDROOMTimerInf *aux;
						aux=nextTimer;

						// **** INSERTA EN LA COLA, NUNCA EL PRIMERO

						while(aux->next&&!found){

							if (aux->next->time<timeValue){

								aux=aux->next;

								//SUPRIMIDO, SIEMPRE EN URGENTE// Va actualizando la prioridad de los timers si es menor (mayor en valor)

								//if(aux->Msg->priority > MsgPrio)
									//aux->Msg->priority=MsgPrio;

							}
							else
								found=true;

						}
						pTimerInf->next=aux->next;
						aux->next=pTimerInf;

						// *******LIBERO MUTEX

						mutexSem.Signal();

				}


			}else {


					// NO HAY NINGUNO EN LA COLA


					EDROOMTimerConfig timerConfig;


					nextTimer=pTimerInf;
					pTimerInf->next=0;

					// SUPRIMIDO SIEMPRE a URGENTE
					//ptimerTask->SetMaxPrioMsg(EDROOMprioURGENT);

					// *******COMPLETO TimerConfig

					//Configuracion del timer

					timerConfig.time=timeValue; // Tiempo en el que se programa el timeout

					timerConfig.timerInf=pTimerInf;

               // *******LIBERO MUTEX

					mutexSem.Signal();


               //Hago que el Actor tome momentaneamente la prioridad URGENTE para conseguir el retorno
					//tras la programación del Timer

					//Mutex
               pActor->mutexSem.Wait();

      			pActor->p_task->SetMaxPrioTmp(EDROOMprioURGENT);

					//FinMutex
					pActor->mutexSem.Signal();


					// *******ENVIO TimerConfig  PORQUE NO HAY NINGUNO

					Pr_Send(timerTask,&timerConfig); // manda el dato al timer

#ifndef _EDROOM_INFORM_AT_NO_NEED_WAIT
					waitActorSem.Wait();
#endif

			}

#ifdef _EDROOM_IS_DEBUG_
		}

#endif

				//Mutex
		pActor->mutexSem.Wait();

      		//Retorna la prioridad del mensaje
      pActor->p_task->RestorePrioMsg();

				//FinMutex
		pActor->mutexSem.Signal();



#ifdef _EDROOM_IS_DEBUG_
         													// en el caso de haberla cambiado
	 } // fin del if regist
	 else
		indice=UINT_MAX-1; // si no esta registrado devuelve UINT_MAX-1

#endif



	 // Se toma el indice como identificador del timer
	 TimerId.indice=indice;

	 return(TimerId);   // devuelve el identificador del timer creado
}

//**********************************************************************


//**********************************************************************
//funcion miembro       InformAt


EDROOMTimer       EDROOMTimingSAP::InformAt(Pr_Time timeValue,EDROOMPriority MsgPrio){
    EDROOMTimer temp;
    temp=InformAt(timeValue,MsgPrio,NULL,0);
	return(temp);   // devuelve el identificador
															  // del timer creado
}


//**********************************************************************




// relative timing functions:

//**********************************************************************
//funcion miembro       InformIn

EDROOMTimer EDROOMTimingSAP::InformIn(Pr_Time timeoutValue,EDROOMPriority MsgPrio){

	 Pr_Time time;
     EDROOMTimer temp;

	 time.GetTime(); // Tomo el tiempo Actual

								//Le sumo el intervalo en segundos
								//para evitar problemas de cambios
								//el periodo de interrupcion

	 time+=timeoutValue.Secns();

     temp=InformAt(time,MsgPrio);
	 return(temp);   // devuelve el identificador
															  // del timer creado
}

//**********************************************************************

//**********************************************************************
//funcion miembro       InformIn


EDROOMTimer EDROOMTimingSAP::InformIn(Pr_Time timeoutValue, EDROOMPriority MsgPrio, void *data,
											M_d * bloqueMemData){
	 Pr_Time time;
     EDROOMTimer temp;

	 time.GetTime(); // Tomo el tiempo Actual

								//Le sumo el intervalo en segundos
								//para evitar problemas de cambios
								//el periodo de interrupcion

	 time+=timeoutValue.Secns();

     temp=InformAt(time,MsgPrio,data,bloqueMemData);
	 return(temp);   // devuelve el identificador
															  // del timer creado
}


//**********************************************************************


//**********************************************************************
//funcion miembro       cancelTimer



void    EDROOMTimingSAP::cancelTimer(EDROOMTimer &timerId){


	//EDROOM_NO_SWITCH_IN_TIMER_CANCEL
   //pActor->mutexSem.Wait();

      		//Pone a urgente
   //pActor->p_task->SetMaxPrioTmp(EDROOMprioURGENT);


				//FinMutex
   //pActor->mutexSem.Signal();


	mutexSem.Wait(); //Captura mutex

	//Tiene que ser un timer correcto


	if (timerId.indice<UINT_MAX-1){ // indice valido

		if (timerId.timeId==timerId.timerInf->timeId){ //con el mismo tiempo

			//***  CANCELAR SI ESTA ENVIADO

			if (timerId.timerInf->state==edroomTimerSent)
					timerId.timerInf->state=edroomTimerCancelled;

			//**** CANCELAR Y DESPERTAR SEMAFORO SI ESTA ACTIVO,
			else  if (timerId.timerInf->state==edroomTimerActive){

					timerId.timerInf->state=edroomTimerCancelled;
					synchrSem.Signal();

		//EDROOM_NO_SWITCH_IN_TIMER_CANCEL
               //waitActorSem.Wait();


		  //**** EXTRAER DE LA COLA SI ESTA READY
			}else if (timerId.timerInf->state==edroomTimerReady){

				// **** NUNCA puede ser EL PRIMERO

				EDROOMTimerInf *aux,*foundTimer;
				bool found;

				found=false;
				aux=nextTimer;

				while(aux->next&&!found){

					if (aux->next->indice==timerId.indice&&aux->next->timeId==timerId.timeId){

						found=true;

						foundTimer=aux->next;

						aux->next=foundTimer->next;

						pActor->mutexSem.Wait();

						EDROOMMessage * auxMsg=(EDROOMMessage *)foundTimer->Msg;

						EDROOMFreeMessage(auxMsg);


						foundTimer->Msg=0;

						pActor->mutexSem.Signal();

						timerInfPool->FreeData(foundTimer);


					}
					else	aux=aux->next;

				}

			}

		  //**** SI ESTA EN OTRO ESTADO, NADA


		}


	}


	timerId.indice=UINT_MAX;

	mutexSem.Signal();  // libera la exclusión mutua.


   //EDROOM_NO_SWITCH_IN_TIMER_CANCEL

   				//Mutex
   //pActor->mutexSem.Wait();

      		//Retorna la prioridad del mensaje
   //pActor->p_task->RestorePrioMsg();

				//FinMutex
   //pActor->mutexSem.Signal();


}


void EDROOMTimingSAP::Start(){


	EDROOMTimingSAP * pTimingSAP;

	pTimingSAP=this;

	Pr_Send(timerTask,&pTimingSAP); // manda el dato al timer

}


void EDROOMTimingSAP::currentTime(Pr_Time &time){time.GetTime();}


Pr_Time EDROOMTimingSAP::currentTime()
{
	Pr_Time time;
	time.GetTime();
	return time;

}



#ifdef _EDROOM_IS_EMBEDDED_


//**********************************************************************
//       EDROOMTimmingMemory SetMemory


void EDROOMTimmingMemory::SetMemory(unsigned int elemNumber, EDROOMTimerInf * TimerInfMem_, unsigned char *TimerInfMemMarks, EDROOMTimeOutMessage* TimeOutMessageMem, unsigned char *TimeOutMessageMemMarks)

	  {


		TimeOutMessMem.SetMemory(elemNumber, TimeOutMessageMem, TimeOutMessageMemMarks);

		TimerInfMem.SetMemory(elemNumber, TimerInfMem_, TimerInfMemMarks);

	  }


//**********************************************************************
//       EDROOMTimerInf SetMemory



void M_d_R_EDROOMTimerInf::SetMemory(unsigned int elemNumber, EDROOMTimerInf* pPoolData,unsigned char * pPoolMarks ){


	M_d_R::SetMemory(elemNumber,pPoolData, pPoolMarks, sizeof (EDROOMTimerInf));

}


EDROOMTimerInf * M_d_R_EDROOMTimerInf::AllocData(unsigned int &ind){

				return (EDROOMTimerInf * )M_d_R::AllocData(ind);
}



#endif



//***************************************************************************
// EDROOMQueue::QueueNode


	EDROOMQueue::QueueNode::QueueNode(){
		p_Msg=0;
		p_next=0;
	}

//***************************************************************************
// EDROOMQueue::QueueHead

	EDROOMQueue::QueueHead::QueueHead(){
		p_first=0;
		p_last=0;
		p_next=0;
	}




//***************************************************************************
// EDROOMQueue

// ****************************************************************************
///////////////DEFINICION DEL CONSTRUCTOR ////////////////////////////////////
// ****************************************************************************

// Revisada y comentada
// PROBADA

EDROOMQueue::EDROOMQueue(unsigned Max_Num_Mess, EDROOMActor   *p_actor_,
					unsigned num_prio_
					
#ifdef _EDROOM_IS_EMBEDDED_

	  ,EDROOMActorMemory *pActorMemory
#endif

					)

#ifdef _EDROOM_IS_DEBUG_

					:newMessSem("Sem_Nuev_Mens",0)
#else


					:newMessSem(0)
#endif


{

	max_num_mess= Max_Num_Mess;

	p_actor=p_actor_;

	bloqueada=0;
	num_mess=0;
	num_prio=num_prio_;


#ifndef _EDROOM_IS_EMBEDDED_

	 // Numero de nodos reservados igual al número máximo de mensajes que se
	 // quiere que la cola soporte

	p_mem_nodes=new M_d_R_QueueNode(Max_Num_Mess);
	 // Numero de cabezas reservadas. Tantas como prioridades
	 // esten definidas en los protocolos que forman el interfaz del Actor.

	queueHeads=new QueueHead[num_prio];


#else

	p_mem_nodes=&pActorMemory->QueueNodesMem;
	queueHeads=pActorMemory->QueueHeadsMem;

#endif

	for(unsigned int i=0; i<num_prio; i++)
		queueHeads[i].priority=(EDROOMPriority)i;


	p_auxQueue=0;

};

// ****************************************************************************
///////////////DEFINICION DEL DESTRUCTOR ////////////////////////////////////
// ****************************************************************************

// Revisada y comentada
// PROBADA

#ifdef _EDROOM_SYSTEM_CLOSE

EDROOMQueue::~EDROOMQueue()
{

#ifndef _EDROOM_IS_EMBEDDED_

	delete(p_mem_nodes); // Devuelvo la memoria reservada
	delete [](queueHeads);


#endif

};


// ****************************************************************************
///////////////DEFINICION DE FREEMEM ////////////////////////////////////
// ****************************************************************************

// Revisada y comentada
// PROBADA

void EDROOMQueue::FreeMem()
{
	QueueNode * aux1_nodo,* aux2_nodo;
	QueueHead *aux1_cabeza;


	//La cabeza de la cola auxiliar esta apuntada por p_auxQueue
	aux1_cabeza=p_auxQueue;

	while(aux1_cabeza)       //Recorro las colas liberando la memoria de los
	{                        //nodos y de las cabezas
				 //Incluyendo las de los mensajes que guardan

		aux1_nodo=aux1_cabeza->p_first;
		while(aux1_nodo)
		{
			EDROOMFreeMessage(aux1_nodo->p_Msg); //Libera la memoria
							 //del mensaje
			aux2_nodo=aux1_nodo;
			aux1_nodo=aux1_nodo->p_next;
			p_mem_nodes->FreeData(aux2_nodo);
		}
		aux1_cabeza=aux1_cabeza->p_next;
	}

									//Recorro la cola de diferidos liberando la memoria
									 //de los nodos


};

#endif


// ****************************************************************************
/////////////////DEFINICION FUNCIONES MIEMBRO PUBLICAS///////////////////////
// ****************************************************************************

// ****************************************************************************
///////////////FUNCION MIEMBRO   LLena ////////////////////////////////////
// ****************************************************************************
	bool EDROOMQueue::Full(){

		 return (num_mess==max_num_mess);
	}
// ****************************************************************************
///////////////FUNCION MIEMBRO   In ////////////////////////////////////
// ****************************************************************************

// Mete un mensaje en la cola de prioridad correspondiente

// Revisada y comentada
// PROBADA

#ifdef _EDROOM_IS_DEBUG_

bool EDROOMQueue::In (EDROOMMessage * pMsg){

		bool error;


#else

void EDROOMQueue::In (EDROOMMessage * pMsg){

#endif



		EDROOMPriority priority=pMsg->priority;


#ifdef _EDROOM_IS_DEBUG_

		if (pMsg==NULL)              // No hay mensaje. ERROR
			error = 1;

		else	{

#endif

			QueueNode *p_new_node; // Apuntador auxiliar para manejar el nuevo nodo

			//mutexSem.Wait();                        // Captura la excluxión mutua de la cola.

#ifdef _EDROOM_IS_DEBUG_

			if (num_mess==max_num_mess){

				// Si la cola está llena falla.

				error=1;

                EDROOMFError <<p_actor->EDROOMName << " Queue FULL" ;
		        EDROOMFError.close();
		        exit(1);

				}
			else {
				error=0;  // No ha habido fallos.

#endif
				p_new_node=p_mem_nodes->AllocData(); // Nuevo nodo
				p_new_node->p_Msg=pMsg;
				p_new_node->p_next=0;

				num_mess++;

				//Añadir en la cola correspondiente a esa prioridad

				if(PutInNode(queueHeads + priority,p_new_node))
					// Si la cola estaba empty hay que actualizar la cola auxiliar
					UpDateAuxQueue(priority);



				// Si el receptor está bloqueado esperando un mensaje y no ha fallado este
				// envio, despierta al receptor.



				if (bloqueada){

					bloqueada=0; // Ya está desbloqueada.
					newMessSem.Signal();

				}


#ifdef _EDROOM_IS_DEBUG_

			}

		// Devuelve si ha fallado el intento de meter el mensaje en la cola.
		}

		return(error);

#endif



};


// ****************************************************************************
///////////////FUNCION MIEMBRO   Out ////////////////////////////////////
// ****************************************************************************

// Saca un mensaje de la cola     // Espera hasta que haya un menjaje en la cola

// Revisada y comentada
// PROBADA


bool EDROOMQueue::Out(EDROOMMessage * &p_Msg){

	 QueueNode *p_node; // apuntador auxiliar

	 EDROOMPriority priority;  // Prioridad del mensaje



	 // Captura la exclusión mutua de la cola
	 p_actor->mutexSem.Wait();

	 // Hay mensajes en la cola

	 if(!p_auxQueue){


				bloqueada++;  // Hay una tarea mas bloqueada esperando.

			// Libera la Exclusión mutua de la cola

			p_actor->mutexSem.Signal();

			// Espera el nuevo mensaje.
			newMessSem.Wait();

			// HA LLEGADO EL NUEVO MENSAJE.

			// Captura la exclusión mutua de la cola

			p_actor->mutexSem.Wait();


	 }


	 // AQUI SEGURO QUE HAY MENSAJE

	// Saca de la cola de mayor prioridad.
	 TakeOutNode(p_node,priority);

	 p_actor->p_task->SetPriority(priority); // Actualizo la prioridad del destino.


	 num_mess--;


	 p_Msg=p_node->p_Msg;

		// Devuelvo la memoria del nodo

	 void * aux_void_p_node= p_node;

	 p_mem_nodes->FreeData(aux_void_p_node);

	 p_node=0;

		// Libera la exclusión mutua.

	 p_actor->mutexSem.Signal();

	 return(0);
		// FIN
};


void EDROOMQueue::UpDateAuxQueue(EDROOMPriority priority){

 QueueHead *aux,*previous,*newHead;

 newHead=queueHeads+priority;

 bool newHeadManaged=0;

 if(!p_auxQueue){

		p_auxQueue=newHead;
		p_auxQueue->p_next=0;


 }else if (p_auxQueue->priority > priority){

			aux=p_auxQueue;
			p_auxQueue=newHead;
			p_auxQueue->p_next=aux;

 }else {
			aux=p_auxQueue;

			while((!newHeadManaged)){

				if  (!aux->p_next){   // No hay siguiente cola

					 aux->p_next= newHead;
					 newHead->p_next=0;    // No hay siguiente
					 newHeadManaged=1;

				} else {

						previous=aux;
						aux=aux->p_next;

						if (aux->priority > priority) {// Hay mas cabezas pero de prioridad
														 // menor (mayor numero)

							 newHead->p_next=aux;  // La mete en la lista de cabezas de colas
							 previous->p_next=newHead;
							 newHeadManaged=1;
						}
				}
			}

 }

}


// ****************************************************************************
///////////////FUNCION MIEMBRO   OutCond ////////////////////////////////////
// ****************************************************************************


// Saca un mensaje de la cola

// No espera. Devuelve un 1 si hay mensaje y un 0 si no lo hay.
/*

unsigned char  EDROOMQueue::OutCond(EDROOMMessage * &p_Msg)
  {
	 int hay_mens;

	 EDROOMPriority priority;

	 QueueNode *ap_nodo; // apuntador auxiliar

	 // Captura la exclusión mutua de la cola
	 mutexSem.Wait();


	 // Mira en la cola de urgentes. Si hay un mensaje saca el primero y libera
	 // la exclusión mutua.

	 if(ap_cola_urgentes->p_first)
		  {
			 ap_nodo=ap_cola_urgentes->p_first;
			 ap_cola_urgentes->p_first=ap_cola_urgentes->p_first->p_next;

			 p_Msg=ap_nodo->p_Msg;
			 p_mem_nodes->Toma((void*)ap_nodo);
			 hay_mens=1;
					 // No es necesario actualizar la prioridad porque ya es urgente.
		  }
	 else
		// Si no mira si num_mess o num_mes_dif es > que cero.
		if ((num_mess>0)||(num_mess_dif>0))
			{
			// Saca de la cola de mayor prioridad.
			Sacar_de_colas(ap_cola_urgentes->p_next,ap_nodo,priority); //Saco de colas no urgentes
																		// hay un elemento por lo menos.

			p_task->SetPriority(priority);//actualizo la prioridad del destino.

			if (num_mess_dif)             // Disminuyo el total de mensajes
				num_mess_dif--;            // que hay en la cola
			else
				num_mess--;

			p_Msg=ap_nodo->p_Msg;
			p_mem_nodes->Toma((void*)ap_nodo);
			hay_mens=1;
			}
		else // Si no, indica que no hay mensaje en cola.
			{

			hay_mens=0;
			}

		// Ya esta localizado el nodo


		// Libera la exclusión mutua.
		mutexSem.Signal();

		return(hay_mens);
		// FIN

	};

  */

// ****************************************************************************
///////////////FUNCION MIEMBRO  GetMaxPriority  ////////////////////////////////////
// ****************************************************************************
// Funcion que devuelve la prioridad del menseje que esta primero en la cola

void  EDROOMQueue::SetToMsgPriority(void){

  EDROOMPriority priority;



  if (p_auxQueue)

		priority=p_auxQueue->priority;

  else

		priority=EDROOMprioMINIMUM; //Minima prioridad puesto que no


  p_actor->p_task->SetPriority(priority); // Actualizo la prioridad del destino.


};




// ****************************************************************************

/////////////////DEFINICION FUNCIONES MIEMBRO PRIVADAS///////////////////////

// ****************************************************************************



// ****************************************************************************
///////////////FUNCION MIEMBRO   Meter_en_cola  //////////////////////////////
// ****************************************************************************

// Revisada y comentada
// PROBADA

bool  EDROOMQueue::PutInNode(QueueHead *head, QueueNode *p_node)
{
	bool empty=1;

	if (head->p_first)  // no esta empty la cola
		{
		head->p_last->p_next=p_node;  // lo mete al final de la cola
		head->p_last=p_node;
		empty=0;
		}
	 else  // Esta empty.
		{
		head->p_first=p_node;

		head->p_last=head->p_first;  // UNICO ELEMENTO

		}
	 return empty;

};


// ****************************************************************************
///////////////FUNCION MIEMBRO   Sacar de colas  //////////////////////////////
// ****************************************************************************


// Saca un nodo de una lista de colas sabiendo que hay por lo menos un nodo
// en alguna cola. POR ESO NO SE CHEQUEA SI NO HA ENCONTRADO NINGUN NODO.

// Revisada y comentada
// PROBADA

void EDROOMQueue::TakeOutNode(QueueNode * &p_node, EDROOMPriority &priority) //Saco de colas no urgentes

{

  //Tomo el primero de la cola de mas prioridad
  p_node=p_auxQueue->p_first;

  priority=p_auxQueue->priority;

  //Actualizo el puntero al primero de la cola de esa prioridad
  p_auxQueue->p_first=p_node->p_next;

  //Si la cola esta empty paso a la siguiente

  if(!p_auxQueue->p_first)

	p_auxQueue=p_auxQueue->p_next;

};


#ifdef _EDROOM_IS_EMBEDDED_


void EDROOMQueue::M_d_R_QueueNode::SetMemory(unsigned int elemNumber, QueueNode * pPoolData,unsigned char * pPoolMarks){

   M_d_R::SetMemory(elemNumber,pPoolData, pPoolMarks, sizeof (EDROOMQueue::QueueNode));


}


EDROOMQueue::QueueNode * EDROOMQueue::M_d_R_QueueNode::AllocData(){

				return (EDROOMQueue::QueueNode * )M_d_R::AllocData();
}

#endif


unsigned long EDROOMCommSAP::idInterface=0;



//***************************************************************************
//								FUNCIONES MIEMBRO DE EDROOMConnectionChannel


//Constructor

EDROOMConnectionChannel::EDROOMConnectionChannel(){

		p_senderPort=0;    //Los dos puertos que forman la conexion
		p_receiverPort=0;

		p_MsgPoolReceiver=0;

			// Reservo memoria para sus mensajes sincronos
			// Solo reservo memoria para tres mensajes sincronos
			// que son los que pueden ser solicitados simultaneamente.
}



// asynchronous communication primitives:

void EDROOMConnectionChannel::send (EDROOMSignal signal, EDROOMPriority priority, void *p_data, M_d * p_DataPool){

	EDROOMMessage *p_Msg;

#ifdef _EDROOM_IS_DEBUG_


	EDROOMErrorComm error=EDROOMNoErrorComm;

//	if(p_QueueReceiver->Full()&& (priority!=EDROOMprioURGENT)){
  	if(p_QueueReceiver->Full()){

		error= EDROOMFullQueueMens;
		if (p_data)
			p_DataPool->FreeData(p_data);

	}else if ((*converFunct)(signal)== EDROOMNoSignal){

		error= EDROOMSignalInvalidate;
		if (p_data)
			p_DataPool->FreeData(p_data);

	}

	else	{  // No hay error

#endif



		// Si la prioridad del mensaje que se envía es mayor que la actual de la tarea emisora,
		//se modifica de forma temporal igualándose a la del mensaje enviado;

		p_senderPort->p_actor->mutexSem.Wait();
		p_senderTask->SetMaxPrioTmp(priority);
		p_senderPort->p_actor->mutexSem.Signal();

#ifdef _EDROOM_IS_DEBUG_

		// Si el pool de mensajes esta vacio la comunicacion se establece, pero el
		// metodo devuelve como error EDROOMEmptyPoolMens

		if (p_MsgPoolReceiver->Empty())
			error=EDROOMEmptyPoolMens;


#endif


		// Reservar memoria para el mensaje
		p_receiverPort->p_actor->mutexSem.Wait();

#ifdef _EDROOM_NO_INHERIT_MSG
		p_receiverTask->SetMaxPrioMsg( priority);
#else
		p_receiverPort->p_actor->SetMaxPrioSendMsg(priority);
#endif

		p_Msg=p_MsgPoolReceiver->AllocData();
		p_receiverPort->p_actor->mutexSem.Signal();

		// Rellenar sus campos;

		// Señal

		p_Msg->signal=(*converFunct)(signal);

		//apuntador al manejador de memoria de los Mensajes del Receptor

		p_Msg->pPoolMess=p_MsgPoolReceiver;

		//dato enviado
		p_Msg->data=p_data;

		//manejador de memoria
		p_Msg->pPoolData=p_DataPool;

		//Prioridad del mensaje

		p_Msg->priority=priority;

		//Completo el campo interf para que apunte al puerto por el que se recibe
		// el mensaje
		p_Msg->edroominterface=p_receiverPort;

		// Se mete el mensaje en la cola asociada a este puerto.

		p_receiverPort->p_actor->mutexSem.Wait();
		p_QueueReceiver->In(p_Msg);
		p_receiverPort->p_actor->mutexSem.Signal();


		// Se restarura la prioridad del mensaje de la tarea emisora

#ifdef _EDROOM_INTERNAL_EVENT_RESTORE_PRIO
		p_senderPort->p_actor->mutexSem.Wait();
		p_senderTask->RestorePrioMsg();
		p_senderPort->p_actor->mutexSem.Signal();
#endif

#ifdef _EDROOM_IS_DEBUG_

	}

    if (error!=EDROOMNoErrorComm){
		EDROOMFError <<p_senderPort->p_actor->EDROOMName << "/" << p_senderPort->name << " .send : " <<EDROOMErrorCommNames[error];
		EDROOMFError.close();
		exit(1);
    }

#endif

	//return error;


}



EDROOMMessage * EDROOMConnectionChannel::invoke(EDROOMSignal signal, EDROOMPriority priority, void *p_data, M_d * p_DataPool){

	EDROOMSynchrMessage *p_Msg;

	Pr_SemaphoreBin *apSemSincr;


#ifdef _EDROOM_IS_DEBUG_

	EDROOMErrorComm error;

    error=EDROOMNoErrorComm;

	// Ver si hay errores
	// Si hay errores el mensaje no se manda

	if ((*converFunct)(signal)== EDROOMNoSignal){

		error= EDROOMSignalInvalidate;

	}

	else {  // No hay error


#endif

            // La prioridad del emisor
            //se modifica de forma temporal a URGENTE

        //Free Mutex    
        p_senderPort->p_actor->mutexSem.Wait();

#ifndef _EDROOM_NO_INHERIT_MSG
        EDROOMPriority maxprio;
            //if the priority of the message is different update
        p_senderTask->SetMaxPrioTmp(priority);
            //Get the priority of the sender task
        maxprio=p_senderTask->GetTaskPrio();
            // the block of the sender
        p_senderPort->p_actor->blockActor=p_receiverPort->p_actor;
#endif


#ifdef _EDROOM_URGENT_INVOKE

        p_senderTask->SetMaxPrioTmp(EDROOMprioURGENT);

#endif

        //Free Mutex
        p_senderPort->p_actor->mutexSem.Signal();



#ifndef _EDROOM_NO_INHERIT_MSG

            //the receptor inherit maxprio

#ifdef _EDROOM_IS_DEBUG_

        p_receiverPort->p_actor->mutexSem.Wait();

        if(p_receiverPort->p_actor->SetMaxPrioInvokeMsg(p_senderPort->p_actor,maxprio))
            error=EDROOMInterblock;

        p_receiverPort->p_actor->mutexSem.Signal();

        if(error==EDROOMNoErrorComm){

#else
            p_receiverPort->p_actor->mutexSem.Wait();
            p_receiverPort->p_actor->SetMaxPrioInvokeMsg(p_senderPort->p_actor,maxprio);
            p_receiverPort->p_actor->mutexSem.Signal();

#endif   /* _EDROOM_IS_DEBUG_ */


#else
            //Pone al receptor a la prioridad del mensaje para evitar la inversion de prioridad

            p_receiverPort->p_actor->mutexSem.Wait();
            p_receiverTask->SetMaxPrioMsg(priority);
            p_receiverPort->p_actor->mutexSem.Signal();

#endif /* _EDROOM_NO_INHERIT_MSG */

		   // Espera a que haya memoria para el mensaje

		   // Tiene su propio semaforo de exclusion mutua y la comunicacion se
    		// realiza con prioridad URGENTE por lo que no hace falta que sea un
	    	// semaforo de recurso

		    p_Msg=p_SynchrMsgManager->AllocData();

	    	// Rellenar sus campos;

    		// Señal

    		p_Msg->signal=(*converFunct)(signal);

		    //apuntador al pool de memoria de los Mensajes sincronos

	    	p_Msg->pPoolMess=p_SynchrMsgManager;

	    	// dato
		    p_Msg->data=p_data;

    		// manejador de memoria del dato

		    p_Msg->pPoolData=p_DataPool;

		    //Prioridad del mensaje

          //The msg inherit

#ifdef _EDROOM_NO_INHERIT_MSG

		    p_Msg->priority=priority;

#else

		    p_Msg->priority=maxprio;  //The msg inherits the priority too.

#endif

		    //Completo el campo interf para que apunte al puerto por el que se recibe
		    // el mensaje

		    p_Msg->edroominterface=p_receiverPort;

		    // Si el pool de mensajes esta vacio la comunicacion se establece, pero el
    		// metodo devuelve como error EDROOMEmptyPoolMens

#ifdef _EDROOM_IS_DEBUG_

	    	if ((*p_p_MsgBack)!=NULL){
		    	  error=EDROOMMsgBackUsed;
			  //EDROOMFreeMsgBack(*p_p_MsgBack);
		    }

#endif

    		//Semaforo para la sincronizacion

	    	apSemSincr=&(p_Msg->semSynchr);

		    // Coloco el apuntador a ese mensaje de retorno en uno de los campos
		    // del mensaje que envio

		    p_Msg->p_MsgBack=p_MessageBack;

		    // Indico al receptor que el emisor esta esperando
		    p_Msg->waiting=1;

		    // Se mete el mensaje el la cola asociada a este puerto.

		    p_receiverPort->p_actor->mutexSem.Wait();
		    p_QueueReceiver->In(p_Msg);
		    p_receiverPort->p_actor->mutexSem.Signal();

    		// Se espera hasta que el receptor haga un reply y señale
	    	// el semaforo en el que va el mensaje.

		    apSemSincr->Wait();

		    // Se restaura la prioridad antigua si se habia modificado

		    p_senderPort->p_actor->mutexSem.Wait();

#ifndef _EDROOM_NO_INHERIT_MSG
            p_senderPort->p_actor->blockActor=0;
#endif
		    p_senderTask->RestorePrioMsg();
		    p_senderPort->p_actor->mutexSem.Signal();

		    // Se retorna el apuntador al mensaje de retorno

#ifndef _EDROOM_NO_INHERIT_MSG
#ifdef _EDROOM_IS_DEBUG_
        }
#endif  /* _EDROOM_IS_DEBUG_ */
#endif /* _EDROOM_NO_INHERIT_MSG */


#ifdef _EDROOM_IS_DEBUG_
	}

    if (error!=EDROOMNoErrorComm){
		EDROOMFError <<p_senderPort->p_actor->EDROOMName << "/" << p_senderPort->name << " .invoke : " <<EDROOMErrorCommNames[error];
		EDROOMFError.close();
		exit(1);
    }

#endif

	return(p_MessageBack);

}




//***************************************************************************
//								FUNCIONES MIEMBRO DE COOMSAP



//***************************************************************************
// *****************        registerInterface *******************************

#ifdef _EDROOM_IS_DEBUG_

int EDROOMCommSAP::registerInterface( EDROOMInterface & interf, EDROOMActor * actor, char * InterfaceName){


		interf.name=InterfaceName;
#ifdef _EDROOM_SYSTEM_CLOSE

		if (interf.registered)             // Si estaba ya registrado
			deregisterInterface(interf);  // hay que desregistrarlo

#endif



#else

int EDROOMCommSAP::registerInterface( EDROOMInterface & interf, EDROOMActor * actor){

#endif


		interf.idInterface=idInterface; //Doy un identificador al interface
		idInterface++;                  //sumo uno para el siguiente identificador

		interf.p_actor=actor; 			// Actor propietario del interface

#ifdef _EDROOM_IS_DEBUG_

		// Registro el interfac
		interf.registered=1;

#endif

		// Falta crear la cola de diferidos para el interface

		return(0); //No hay error posible de momento
}

//***************************************************************************
// *****************        deregisterInterface *******************************




ConnectionId EDROOMCommSAP::connect(EDROOMActor *actor,EDROOMInterface &inter1,
																		EDROOMInterface &inter2, EDROOMSignal (f1) (EDROOMSignal ), EDROOMSignal  (f2) (EDROOMSignal )){
	ConnectionId nuevaConn=0;

				//Si estan registrados los puertos.

#ifdef _EDROOM_IS_DEBUG_

	if (inter1.registered&&inter2.registered){

#endif

		// Aqui se podia añadir algo sobre la compatibilidad de los protocolos
		//

		nuevaConn=actor->p_ConnectPool->AllocData(); //nueva conexion

		nuevaConn->channel1.p_senderPort=&inter1;     //puertos que la forman

		nuevaConn->channel1.p_receiverPort=&inter2;     //puertos que la forman

		nuevaConn->channel2.p_senderPort=&inter2;     //puertos que la forman

		nuevaConn->channel2.p_receiverPort=&inter1;     //puertos que la forman

		// A los puertos se les da tambien el puntero a la conexion

		inter1.p_channel=&nuevaConn->channel1;
		inter2.p_channel=&nuevaConn->channel2;

		nuevaConn->p_ConnectPool=actor->p_ConnectPool;	//Apuntador al bloque de memoria
																	//de las conexiones

		// Apuntador a la tareas propietarias de los puertos

		nuevaConn->channel1.p_senderTask=inter1.p_actor->p_task;
		nuevaConn->channel1.p_receiverTask=inter2.p_actor->p_task;
		nuevaConn->channel2.p_senderTask=inter2.p_actor->p_task;
		nuevaConn->channel2.p_receiverTask=inter1.p_actor->p_task;

		// Apuntador a la colas de mensajes de los actores propietarios de los puertos

		nuevaConn->channel1.p_QueueReceiver=&inter2.p_actor->msgQueue;
		nuevaConn->channel2.p_QueueReceiver=&inter1.p_actor->msgQueue;


		// Apuntador a los manejadores de memoria de los mensajes

		nuevaConn->channel1.p_MsgPoolReceiver=inter2.p_actor->p_MsgPool;
		nuevaConn->channel2.p_MsgPoolReceiver=inter1.p_actor->p_MsgPool;

		//FUNCIONES DE CONVERSION DE SEÑALES

		nuevaConn->channel1.converFunct=f1;
		nuevaConn->channel2.converFunct=f2;

		// Puntero al MsgBack
		nuevaConn->channel1.p_p_MsgBack=&inter1.p_actor->MsgBack;
		nuevaConn->channel1.p_MessageBack=&inter1.p_actor->MessageBack;


		nuevaConn->channel2.p_p_MsgBack=&inter2.p_actor->MsgBack;
		nuevaConn->channel2.p_MessageBack=&inter2.p_actor->MessageBack;

		nuevaConn->channel1.p_SynchrMsgManager=&inter2.p_actor->SynchrMsgManager;
		nuevaConn->channel2.p_SynchrMsgManager=&inter1.p_actor->SynchrMsgManager;

					//marco los puertos como conectados

#ifdef _EDROOM_IS_DEBUG_

		inter1.connected=1;
		inter2.connected=1;

	 }

#endif


	 return(nuevaConn);
}



#ifdef _EDROOM_SYSTEM_CLOSE


int EDROOMCommSAP::deregisterInterface(EDROOMInterface & interf){



#ifdef _EDROOM_IS_DEBUG_
		int error=1;

		if (interf.registered){


			interf.registered=0; //Lo desregistro
			error=0;
		  //	delete (interf.apMemMensDi); //borro la memoria que tiene


		}
		return(error); //No hay error posible de momento


#else

		return 0;

#endif



}





int EDROOMCommSAP::disconnect(ConnectionId &connection){

#ifdef _EDROOM_IS_DEBUG_

	int error=1;

	if (connection){ //Si hay conexion establecida

		connection->channel1.p_senderPort->connected=0;     //Se marcan como desconectados los
		connection->channel2.p_senderPort->connected=0;     //Se marcan como desconectados los

#endif
		connection->p_ConnectPool->FreeData(connection);	//Se libera la memoria de la conexion


#ifdef _EDROOM_IS_DEBUG_
		error=0; //no hay error
		}
	return(error);

#else
	return 0;

#endif
}



#endif



#ifdef _EDROOM_IS_EMBEDDED_

void M_d_R_EDROOMConnection::SetMemory(unsigned int elemNumber, EDROOMConnection* pPoolData,unsigned char * pPoolMarks){

		M_d_R::SetMemory(elemNumber,pPoolData, pPoolMarks, sizeof (EDROOMConnection));


}


EDROOMConnection * M_d_R_EDROOMConnection::AllocData(){

				return (EDROOMConnection * )M_d_R::AllocData();

}


#endif





//**************************************************************************
// IMPLEMENTACION DE LOS ACTORES DE UN MODELO EDROOM BASADAS EN LAS
// PRIMITIVAS DE TIEMPO REAL DEFINIDAS EN prim_pp.h
//**************************************************************************

//*****************************************************************//
//                                                      CLASE Actor                                //
//*****************************************************************//


//******************** CONSTRUCTOR ************************//


// DEFINICION DEL CONSTRUCTOR DE LA CLASE EDROOMActor




#ifdef _EDROOM_IS_DEBUG_

EDROOMActor::EDROOMActor(const char * const pname, const char * const pnameContainer,
										  unsigned numPri, unsigned numCon,int numMaxM,
										 EDROOMPriority prioTask,unsigned stack

#ifdef _EDROOM_IS_EMBEDDED_
										,EDROOMActorMemory * pActorMemory

#endif


										 ): mutexSem("Semaforo Actor")
										 ,actorTask(EDROOMTareaActor,(char *) pname,prioTask,stack)
										 ,SynchrMsgManager(&SynchrMsg)

#ifdef _EDROOM_IS_EMBEDDED_
										 ,	msgQueue(numMaxM, this, numPri, pActorMemory)
#else

										 ,	msgQueue(numMaxM, this, numPri)
															// Reservo
															// la cola de mensajes con
#endif /* _EDROOM_IS_EMBEDDED_ */														// un maximo de mensajes dado
															// por numMaxMens y un numero de
															// prioridades dado por numPrior


										 ,EDROOMName(pname)
										 ,EDROOMNameContainer(pnameContainer)



#else

EDROOMActor::EDROOMActor(  unsigned numPri, unsigned numCon,int numMaxM,
										 EDROOMPriority prioTask,unsigned stack

#ifdef _EDROOM_IS_EMBEDDED_
										,EDROOMActorMemory * pActorMemory	 ):

										 SynchrMsgManager(&SynchrMsg)
										 ,msgQueue(numMaxM, this, numPri, pActorMemory)
										  ,actorTask(EDROOMTareaActor,prioTask,stack)

#else

										):
										SynchrMsgManager(&SynchrMsg)
										 ,msgQueue(numMaxM, this, numPri)
										 ,actorTask(EDROOMTareaActor,prioTask,stack)


#endif


#endif



										 {


	p_task=&actorTask; // Todavia no tiene una tarea que ejecute su comportamiento.

	numMaxMens=numMaxM; // Numero maximo de mensajes que mantendra este actor
							// en su cola de mensajes

#ifdef _EDROOM_IS_EMBEDDED_

	p_MsgPool = &pActorMemory->MessagesMem; // Tomo la memoria reservada

#else

	p_MsgPool = new M_d_R_EDROOMMessage(numMaxMens); // Reservo el bloque
										//de memoria para los mensajes que recibira
												//este actor.
#endif


	numConex=numCon;        // Numero de conexiones entre los actores componentes
								// de ese actor, esta determinado por el diseño del
								// actor.

#ifndef _EDROOM_IS_EMBEDDED_

	p_ConnectPool = new M_d_R_EDROOMConnection(numCon); // Reservo el bloque
												//de memoria para los mensajes que recibira
												//este actor.

#else

	p_ConnectPool = &pActorMemory->ConnectionsMem; // Tomo la memoria reservada


#endif


	numPrior=numPri;  // Numero de prioridades de los mensajes que trata.

	Msg=0;               //Inicialmente no hay ningun mensaje

	MsgBack=0;               //Inicialmente no hay ningun mensaje




#ifdef _EDROOM_SYSTEM_CLOSE

	finished=0;   // La tarea no ha terminado

#endif

#ifdef _EDROOM_HAND_EXCEP_

	NewException=0;

#endif


#ifndef _EDROOM_NO_INHERIT_MSG

    blockActor=0;
#endif

}

					// para liberar la memoria del actor
					// No se hace en el constructor para permitir que se libere la
					// de forma distribuida


#ifdef _EDROOM_SYSTEM_CLOSE

void EDROOMActor::FreeMem(){

	  mutexSem.Wait();
	  EDROOMFreeMessage(Msg);

								  //Libero la memoria reservada
	  msgQueue.FreeMem(); // PRIMERO LA COLA DE MENSAJES


#ifndef _EDROOM_IS_EMBEDDED_

	  delete(p_MsgPool); //DESPUES LOS PROPIOS MENSAJES

#endif

	  finished=1;

	  mutexSem.Signal();


	};


// DEFINICION DEL DESTRUCTOR DE LA CLASE EDROOMActor


EDROOMActor::~EDROOMActor(){

#ifndef _EDROOM_IS_EMBEDDED_

	  delete(p_ConnectPool); // CONEXIONES

#endif


}


//******************** METODOS PUBLICOS ************************//

// DEFINICION DEL METODO ActorTerminado DE LA CLASE Actor



bool EDROOMActor::EDROOMActorTerminado(){
		return (finished);
}


#endif



void EDROOMActor::EDROOMDestroy(){


	  EDROOMMessage *Msg;

	  mutexSem.Wait();

	  Msg=p_MsgPool->AllocData(); //Tomo un mensaje nuevo

	  Msg->signal=1 ;   // EDROOMDestroy

	  Msg->pPoolMess=p_MsgPool;

	  //No hay dato enviado
	  Msg->data=NULL;

	  //No hay manejador de memoria
	  Msg->pPoolData=0;

	  //Prioridad del mensaje establecida por defecto

	  Msg->priority=EDROOMprioVeryHigh;

     actorTask.SetMaxPrioTmp( EDROOMprioVeryHigh);

	  msgQueue.In(Msg); // METO EN LA COLA DE MENSAJES

	  mutexSem.Signal();


}

// DEFINICION DEL METODO Start DE LA CLASE EDROOMActor

// El metodo Start crea una tarea a partir de la funcion miembro Comportamiento

// El parametro prioTask es la prioridad inicial de las tareas



int EDROOMActor::EDROOMStart(){
	EDROOMActor *ap_actor;

	ap_actor=this; // Le manda un puntero a este actor a la tarea para
						// que ejecute el comportamiento

	Pr_Send(*p_task,&ap_actor);


	return (1); // Sin errores
}



//******************** METODOS PUBLICOS ************************//

// DEFINICION DEL METODO NewMsg DE LA CLASE EDROOMActor

  // Funcion miembro que permite sacar un mensaje  de la cola y hace que Msg
 // apunte a ese mensaje a la vez que libera la memoria del anterior
 // mensaje al que estaba apuntando Msg

void EDROOMActor::EDROOMNewMsg(){

	 do{


		//Si no libero la memoria del mensaje anterior
		mutexSem.Wait();
		EDROOMFreeMessage(Msg);


			//Cambio la prioridad de la tarea a la del siguiente mensaje
			//que hay en la cola (si no hay ninguno la prioridad sera la minima)
			//Esto permite que el resto de los actores tengan la oportunidad
			//de enviar sus mensajes mas prioritarios a este mismo actor

		msgQueue.SetToMsgPriority();
		mutexSem.Signal(); //Libero aqui la exclusion mutua para que se haga efectiva
								//la posible entrada de otros


		msgQueue.Out(Msg); // Tomo un nuevo mensaje de la cola y hago que
									 // Msg apunte a el.

	 }while(Msg->cancelled());  // Si el mensaje esta cancelado repito la operacion

}

//******************** METODOS PUBLICOS ************************//

// DEFINICION DEL METODO NewMsgWithExc DE LA CLASE EDROOMActor

  //Anade a NewMsg la capacidad de tratar con interrupciones

#ifdef _EDROOM_HAND_EXCEP_

void EDROOMActor::EDROOMNewMsgWithExc(){

	 do{

		//Si no libero la memoria del mensaje anterior
		mutexSem.Wait();

		if (NewException){

			MsgExc=Msg;

			Msg=p_MsgPool->AllocData();
			Msg->signal= 2;  // EDROOMException es siempre 2
			Msg->data=NULL;
			Msg->pPoolData=NULL;
 			NewException=false;

			mutexSem.Signal(); //Libero aqui la exclusion mutua para que se haga efectiva
								//la posible entrada de otros

		}
		else {
			EDROOMFreeMessage(Msg);


			//Cambio la prioridad de la tarea a la del siguiente mensaje
			//que hay en la cola (si no hay ninguno la prioridad sera la minima)
			//Esto permite que el resto de los actores tengan la oportunidad
			//de enviar sus mensajes mas prioritarios a este mismo actor

			msgQueue.SetToMsgPriority();

			mutexSem.Signal(); //Libero aqui la exclusion mutua para que se haga efectiva
								//la posible entrada de otros

								// Lo puedo hacer antes del MsgOut porque este tambien respeta la exclusion
                        //Mutua

			msgQueue.Out(Msg); // Tomo un nuevo mensaje de la cola y hago que
									 // Msg apunte a el.


		}

	 }while(Msg->cancelled());  // Si el mensaje esta cancelado repito la operacion

}

#endif


/**
//EDROOMTraceTrans
// Include info to Trace the execution
*/

#ifdef _EDROOM_IS_DEBUG_

void EDROOMActor::EDROOMTraceTrans(EDROOMTransId & TransId, EDROOMSignal signal){

        Pr_Time currentTime;

        mutexSem.Wait();  //Take the URGENT prio to avoid the preemption
        p_task->SetMaxPrioTmp(EDROOMprioURGENT);


        currentTime.GetTime();
        EDROOMFDeb << "\tEndEvent\n"<<"MsgIn"  ;
        EDROOMFDeb << "\t" << currentTime.GetTicks();
        EDROOMFDeb << "\t" << EDROOMName  ;
        EDROOMFDeb << "\t" << signal;
        EDROOMFDeb << "\t" << TransId.name ;
        EDROOMFDeb << "\t" << TransId.contextLevel;

        EDROOMFDeb.close();
        EDROOMFDeb.open("roomfdeb.lst",ios::app) ;

        p_task->RestorePrioMsg(); //Restore the situation
        mutexSem.Signal();

}

void EDROOMActor::EDROOMTraceStateEntry(){

        Pr_Time currentTime;

        mutexSem.Wait();  //Take the URGENT prio to avoid the preemption
        p_task->SetMaxPrioTmp(EDROOMprioURGENT);
        currentTime.GetTime();

        EDROOMFDeb << "\t" << EDROOMName  ;
        EDROOMFDeb << "\t" << currentTime.GetTicks();
        
        EDROOMFDeb.close();
        EDROOMFDeb.open("roomfdeb.lst",ios::app) ;

        p_task->RestorePrioMsg(); //Restore the situation
        mutexSem.Signal();

}

#endif



#ifndef _EDROOM_NO_INHERIT_MSG

/**
//SetMaxPrioSendMsg
// Fix the priority and Propagate to the blockActor
*/

void EDROOMActor::SetMaxPrioSendMsg( EDROOMPriority  priority){
        p_task->SetMaxPrioMsg(priority);
        //Propagate the priority to avoid the priority inversion
        if(blockActor){
                blockActor->mutexSem.Wait();
                blockActor->SetMaxPrioSendMsg(priority);
                blockActor->mutexSem.Signal();

        }

}

/**
//SetMaxPrioInvokeMsg
// Fix the priority and Propagate to the blockActor
// Check if there is Interblock
*/

bool EDROOMActor::SetMaxPrioInvokeMsg( EDROOMActor * actor, EDROOMPriority  priority){
        bool InterBlock;

        p_task->SetMaxPrioMsg(priority);
        //Propagate the priority to avoid the priority inversion
        if(blockActor){
                if(blockActor!=actor){
                        blockActor->mutexSem.Wait();
                        InterBlock=blockActor->SetMaxPrioInvokeMsg(actor, priority);
                        blockActor->mutexSem.Signal();
                }else InterBlock=true;

        }else
                InterBlock=false;

        return  InterBlock;

}

#endif



#ifdef _EDROOM_IS_EMBEDDED_

//*****************************************************************//
//                                    CLASE EDROOMActorMemory                                //
//*****************************************************************//


void EDROOMActorMemory::SetMemory(unsigned int connectNumber_, EDROOMConnection * ConnectionsMem_, unsigned char * ConnectionsMemMarks,

					unsigned int messagesNumber_,EDROOMMessage  * MessagesMem_, unsigned char * MessagesMemMarks_, EDROOMQueue::QueueNode * QueueNodesMem_, unsigned char * QueueNodesMemMarks_,

					EDROOMQueue::QueueHead *QueueHeadsMem_){



				ConnectionsMem.SetMemory(connectNumber_, ConnectionsMem_,  ConnectionsMemMarks);
				MessagesMem.SetMemory(messagesNumber_, MessagesMem_, MessagesMemMarks_);
				QueueNodesMem.SetMemory(messagesNumber_, QueueNodesMem_, QueueNodesMemMarks_);


				QueueHeadsMem=QueueHeadsMem_;

}

#endif





#ifdef _EDROOM_HAND_EXCEP_

EDROOMExceptionSAP::EDROOMExceptionSAP(EDROOMActor *pActor_){

	pActor=pActor_;

};

EDROOMExceptionSAP::newException(){
	pActor->mutexSem.Wait();
	pActor->NewException=true;

   #ifdef _EDROOM_NO_INHERIT_MSG

		pActor->actorTask.SetMaxPrioMsg(EDROOMprioURGENT);

	#else

		pActor->SetMaxPrioSendMsg(EDROOMprioURGENT);

	#endif

	pActor->mutexSem.Signal();

	//¿Cambiar prioridad a URGENTE?

};

#endif
