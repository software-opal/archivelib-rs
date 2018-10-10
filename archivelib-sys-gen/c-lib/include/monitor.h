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


#ifndef _MONITOR_H
#define _MONITOR_H

#if defined( __cplusplus )

/*
 * Most of our enumerated types are defined in ALDEFS.H so C programs
 * have easy access to them.  But C and VB have no use for these
 * values, because they are only used by members of ALMonitor,
 * they can't be passed to C or VB procedures.
 *
 * This type defines the various messages that are passed to 
 * ALMonitor::ArchiveOperation() during progress of an
 * archiving operation.
 */

enum ALArchiveOperation {
    AL_ARCHIVE_OPEN,
    AL_ARCHIVE_CLOSE,
    AL_EXTRACTION_OPEN,
    AL_EXTRACTION_CLOSE,
    AL_INSERTION_OPEN,
    AL_INSERTION_CLOSE,
    AL_COPY_OPEN,
    AL_COPY_CLOSE,
    AL_START_DIRECTORY_WRITE,
    AL_END_DIRECTORY_WRITE,
    AL_START_DIRECTORY_READ,
    AL_END_DIRECTORY_READ
};

/*
 * A forward declaration.
 *
 */
class AL_CLASS_TYPE ALEntry;

/*
 * class ALMonitor
 *
 * DESCRIPTION
 *
 *  The ALMonitor class is used primarily for providing user interface
 *  information during archiving or other operations.  Each ALEntryList
 *  object has an ALMonitor attached to it.  When the archive operations
 *  are performed, the ALMonitor object is called using its two functions,
 *  Progress() and ArchiveOperation().  Progress() is used to update
 *  progress information, such as byte count or percent complete.  The
 *  archive operation is functioned at various times, such as when files
 *  are opened, archives are opened, files are closed, etc.
 *
 *  Good examples of how derived classes do all this stuff can be found in
 *  BARGRAPH.CPP and WINMON.CPP.
 *
 * DATA MEMBERS
 *
 *  mlJobSize       : The total size of the job being monitored.  If the
 *                    monitor is in AL_MONITOR_JOBS mode, we care about
 *                    this. The archive base class has to set up this
 *                    member before the archiving procedure starts.
 *
 *  mlJobSoFar      : The total number of bytes that have already been
 *                    processed so far in this job.  This number is
 *                    updated after each file is completely processed.  It
 *                    does not reflect work done on the current file.
 *
 *  mlObjectSize    : The size of the current object being processed.  This
 *                    is usually set up by the archive function at the start
 *                    of processing for the current file.  If this number is
 *                    set to -1, it means we have to check on it, since
 *                    the archiving program was to lazy to open the file and
 *                    check on.
 *
 *  mlObjectStart   : The starting offset of the current object in the
 *                    file being processed.  Sometimes the object we are 
 *                    working on might be in an archive, in which case
 *                    we need to know where it starts.
 *
 *  mlByteCount     : This private member contains the byte count for all
 *                    the data processed so far.  Derived classes can
 *                    call Progress() for the base class, and it will
 *                    calculate this number.  The number will be for
 *                    the entire job when we are in AL_MONITOR_JOB mode,
 *                    but just for the current object when we are in 
 *                    AL_MONITOR_OBJECTS mode.
 *
 *  miRatio         : The percentage of processing we have completed, an
 *                    integer that hopefully ranges from 0 to 100.  This
 *                    number can also be calculated by calling Progress()
 *                    in the base class.  It will differ depending on whether
 *                    we are in AL_MONITOR_JOB mode or AL_MONITOR_OBJECTS.
 *
 *  miMonitorType   : Either AL_MONITOR_OBJECT or AL_MONITOR_JOBS, depending
 *                    on what was specified in the constructor.
 *
 * MEMBER FUNCTIONS
 *
 *  ALMonitor()        : The constructor.
 *  ~ALMonitor()       : The virtual destructor.
 *  operator new()     : The memory allocation operator used when the
 *                       library resides in a DLL.
 *  Progress()         : The routine called periodically to update progress
 *                       indicators.  The base class version just makes
 *                       calculations.
 *  ArchiveOperation() : The routine called at various points during an
 *                       archive operation.  Does nothing worthwhile in the
 *                       base class.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALMonitor
{
/*
 * Constructors, destructors, friends, declarations
 */
    friend class AL_CLASS_TYPE ALArchiveBase;
    friend class AL_CLASS_TYPE ALStorage;

    public :
        AL_PROTO ALMonitor( ALMonitorType monitor_type );
        virtual AL_PROTO ~ALMonitor();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * The copy constructor and assignment operator do not exist.
 */
    protected :
        AL_PROTO ALMonitor( ALMonitor AL_DLL_FAR &);
        ALMonitor AL_DLL_FAR & AL_PROTO operator=( ALMonitor AL_DLL_FAR & );
/*
 * Member functions
 */
    protected :
        virtual void AL_PROTO Progress( long mlObjectTell, 
                                        ALStorage AL_DLL_FAR & object );
        virtual void AL_PROTO 
        ArchiveOperation( enum ALArchiveOperation operation,
                          ALArchiveBase AL_DLL_FAR *archive,
                          ALEntry AL_DLL_FAR *job );
/*
 * Data members, not sure if we should keep these public or not
 */
    public :
        long mlJobSize;
        long mlJobSoFar;
        long mlObjectSize;
        long mlObjectStart;
/*
 * Calculated by ALMonitor::Progress()
 */
        long mlByteCount;
        int miRatio;

    public :
        const ALMonitorType miMonitorType;
        AL_CLASS_TAG( _ALMonitorTag );
};

#endif /* #if defined( __cplusplus ) */

#endif /* #ifndef _MONITOR_H         */
