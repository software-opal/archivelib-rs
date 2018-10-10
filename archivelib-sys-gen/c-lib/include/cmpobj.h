/*
Copyright 1990-2008 Light Infocon Tecnologia S/A

Este arquivo é parte do programa LightBase - Banco de Dados Textual Documental

O LightBase é um software livre; você pode redistribui-lo e/ou modifica-lo dentro 
dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software 
Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA 
GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO 
EM PARTICULAR. Veja a Licença Pública Geral GNU para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU versao 2, sob o 
título "LICENCA.txt", junto com este programa, se não, escreva para a Fundação do 
Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/


#ifndef _CMPOBJ_H
#define _CMPOBJ_H

#if defined( __cplusplus )

/*
 * class ALCompressedObject
 *
 * DESCRIPTION
 *
 * This is the class definition for ALCompressedObject.  A compressed
 * object is a very simple artifact that contains some compressed data,
 * a checksum, and a couple of length variables.  No comments, no engine
 * data, no archive data, nothing else.  This give this type of object
 * a very low overhead.  It also assumes that when you create a compressed
 * object using a storage object and a compression engine that you will
 * know what type of storage object and compression engine to use when
 * expanding it.
 *
 * If you decide you want to add some private data to your compressed
 * object, it isn't hard.  Just derive a new class, and implement the
 * two virtual functions defined here to read and write your own private
 * data during compression and decompression.
 *
 * DATA MEMBERS
 *
 *  mpCompressionEngine  : A pointer to the compression engine that will
 *                         be used by this object.  This is assigned when
 *                         the object is created, and used to insert or
 *                         extract objects.
 *
 *  mpStorageObject      : A pointer to the storage object where this
 *                         object will live.
 *
 *  mStatus              : A standard ArchiveLib status object.
 *
 * MEMBER FUNCTIONS
 *
 *  ALCompressedObject()  : The only constructor for ALCompressedObject.
 *  ~ALCompressedObject() : The virtual destructor.
 *  operator new()        : Memory allocation operator, used in DLL only.
 *  WriteHeaderData()     : Virtual function to allow for storage of
 *                          customized data in the object header.
 *  ReadHeaderData()      : The virtual complement to the previous function,
 *                          lets you read in some customized data.
 *  Insert()              : Insert a single storage object into the Compressed
 *                          object.
 *  Extract()             : Extract the storage object from the Compressed
 *                          object.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALCompressedObject {
/*
 * Constructors, destructors, declarations, friends
 */
    public :
        AL_PROTO ALCompressedObject( ALStorage AL_DLL_FAR & storage_object,
                                     ALCompressionEngine AL_DLL_FAR & engine );
        virtual AL_PROTO ~ALCompressedObject();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * Define the copy constructor and assignment operator here, that way
 * the compiler won't attempt to.
 */
    protected :
        AL_PROTO ALCompressedObject( ALCompressedObject AL_DLL_FAR & );
        ALCompressedObject AL_DLL_FAR & AL_PROTO operator = ( ALCompressedObject AL_DLL_FAR & );
/*
 * Member functions
 */
    protected :
        virtual int AL_PROTO WriteHeaderData( ALStorage AL_DLL_FAR * storage = 0 );
        virtual int AL_PROTO ReadHeaderData( ALStorage AL_DLL_FAR * storage = 0 );
    public :
        int AL_PROTO Insert( ALStorage AL_DLL_FAR &input_object );
        int AL_PROTO Extract( ALStorage AL_DLL_FAR &output_object );
/*
 * Data members
 */
    protected :
        ALCompressionEngine AL_DLL_FAR *mpCompressionEngine;
        ALStorage AL_DLL_FAR *mpStorageObject;

    public :
        ALStatus mStatus;
        AL_CLASS_TAG( _ALCompressedObjectTag );
};

#endif /* #if defined( __cplusplus ) */

#endif /* #ifdef _CMPOBJ_H           */
