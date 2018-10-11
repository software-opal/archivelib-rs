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


#include "arclib.h"
#pragma hdrstop

//
// void * ALMonitor::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes needed to create a new ALMonitor object.
//
// RETURNS
//
//  A pointer to the newly allocated storage area, or 0 if no storage
//  was available.
//
// DESCRIPTION
//
//  When using a DLL, it is easy to get into a dangerous situation when
//  creating objects whose ctor and dtor are both in the DLL.  The problem
//  arises because when you create an object using new, the memory for
//  the object will be allocated from the EXE.  However, when you destroy
//  the object using delete, the memory is freed inside the DLL.  Since
//  the DLL doesn't really own that memory, bad things can happen.
//
//  But, you say, won't the space just go back to the Windows heap regardless
//  of who tries to free it?  Maybe, but maybe not.  If the DLL is using
//  a subsegment allocation scheme, it might do some sort of local free
//  before returning the space to the windows heap.  That is the point where
//  you could conceivably cook your heap.
//
//  By providing our own version of operator new inside this class, we
//  ensure that all memory allocation for the class will be done from
//  inside the DLL, not the EXE calling the DLL.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

#if defined( AL_BUILDING_DLL )
void AL_DLL_FAR * AL_PROTO ALMonitor::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// ALMonitor::ALMonitor( ALMonitorType monitor_type )
//
// ARGUMENTS:
//
//  monitor_type : One of the enumerated types from ALDEFS.H.  The only
//                 two types supported are AL_MONITOR_OBJECTS and
//                 AL_MONITOR_JOB.
// RETURNS
//
//  Nothing, this is a constructor.
//
// DESCRIPTION
//
//  This function is called when one of the derived classes is creating
//  a new monitor. (It could be called directly, but you aren't likely
//  to instantiate an ALMonitor.)  It has only one thing to do, which
//  is to initialize the miMonitorType data member.  This data member
//  is a const member, so it has to be initialized in an initializer list.
//  It's nice to make it const, because then you can leave it public and
//  nobody gets to jack with it.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A    : First release
//
//   January 9, 1995 1.01A : Decided it would be good to initialize these
//                           members.
//
AL_PROTO ALMonitor::ALMonitor( ALMonitorType monitor_type )
    : miMonitorType( monitor_type )
{
    mlObjectStart = 0;
    mlObjectSize = -1;
}

//
// ALMonitor::~ALMonitor()
//
// ARGUMENTS:
//
//  None, destructor.
//
// RETURNS
//
//  Likewise, none for a destructor.
//
// DESCRIPTION
//
//  The ALMonitor destructor doesn't have to clean up any dynamic
//  storage or anything like that.  As a consequence, all we do is
//  check the validity of this in debug mode.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

AL_PROTO ALMonitor::~ALMonitor()
{
    AL_ASSERT( GoodTag(), "~ALMonitor: attempt to delete invalid object" );
}

//
// void ALMonitor::Progress( long object_tell,
//                           ALStorage & object )
//
// ARGUMENTS:
//
//  object_tell  : The current offset withing the object being compressed,
//                 expanded, copied, or processed.
//
//  object       : A reference to the storage object being processed.
//
// RETURNS
//
//  None.
//
// DESCRIPTION
//
//  This is a virtual function.  ALMonitor::Progress() gets called from
//  YieldTime() inside a storage object, which happens pretty
//  frequently.  Normally the derived class will have its own version
//  of Progress(), so this guy won't get called directly.
//
//  However, most of the derived versions of Progress() will go ahead and
//  call this version anyway.  Why?  Because this guy calculates the values
//  of miRatio and mlByteCount for you.
//
//  The calculated values of miRatio and mlByteCount will differ depending
//  on whether the monitor is of type AL_MONITOR_JOB or AL_MONITOR_OBJECTS.
//
//  In AL_MONITOR_OBJECTS mode, the byte count is going be calculated by
//  taking the current offset of the object and subtracting the starting
//  position of the object.  We have to subtract out the starting position,
//  because sometimes we are going to be monitoring an object that resides
//  in an archive, and its starting position will not be at location 0.
//
//  If we are in AL_MONITOR_JOB mode, the byte count is going to be the
//  same as referred to above, plus the value of mlJobSoFar.  That data
//  member contains the total number of bytes processed in previous objects
//  in this job.  That figure is updated after each object is processed,
//  but not by this class. ALArchiveBase does this for ordinary archiving
//  operations, you can look at that code for hints on how to do this
//  yourself.
//
//  Calculating the ratio is pretty easy.  If you are in AL_MONITOR_OBJECTS
//  mode, you just divide the byte count by the object size.  If you are
//  in AL_MONITOR_JOB mode, you divide the byte count by the job size.  Once
//  again, the job size will have been calculated in advance by whatever
//  process is performing the compression/expansion operation.
//
//  Note that there is one tricky bit here.  If the object size was set to
//  -1 by the calling program, it means this routine has to go out and
//  get the size.  This convenience cuts down on code in the high level
//  routine.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

void AL_PROTO ALMonitor::Progress( long object_tell,
                                   ALStorage AL_DLL_FAR & object )
{
    mlByteCount = object_tell - mlObjectStart;
    if ( mlObjectSize == -1 )
        mlObjectSize = object.GetSize();
    if ( miMonitorType == AL_MONITOR_JOB ) {
        mlByteCount += mlJobSoFar;
        if ( mlJobSize == 0 )
            miRatio = -1;
        else
            miRatio = (int)( 100 * mlByteCount / mlJobSize );
    } else {
        if ( mlObjectSize == 0 )
            miRatio = -1;
        else
            miRatio = (int)(100 * mlByteCount / mlObjectSize );
    }
}

//
// void ALMonitor::ArchiveOperation( ALArchiveOperation,
//                                   ALArchiveBase *,
//                                   ALEntry * )
//
// ARGUMENTS:
//
//  None.  There are actually three arguments passed to this function,
//         but we ignore them here.  Derived classes may do something.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  Derived classes override this function to print informative information
//  about various archiving operations.  The base class does absolutely
//  nothing with this information, it is a do-nothing function.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

void AL_PROTO ALMonitor::ArchiveOperation( ALArchiveOperation,
                                           ALArchiveBase AL_DLL_FAR *,
                                           ALEntry AL_DLL_FAR * )
{
}



