/*
 * Ubitrack - Library for Ubiquitous Tracking
 * Copyright 2006, Technische Universitaet Muenchen, and individual
 * contributors as indicated by the @authors tag. See the
 * copyright.txt in the distribution for a full listing of individual
 * contributors.
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

/**
* @ingroup api
* @file
* Implements a basic interface to the ubitrack dataflow network.
*
* @author Ulrich Eck <ueck@net-labs.de
*/
#include <utFacade/Config.h>
#ifdef ENABLE_BASICFACADE

#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <boost/bind.hpp>

#include <log4cpp/Category.hh>

#ifdef HAVE_OPENCV
#include <utVision/Image.h>
#endif

#include "BasicFacadeTypesPrivate.h"
#include "BasicFacadeComponentsPrivate.h"
#include "BasicFacadePrivate.h"

#include "AdvancedFacade.h"
#include "DataflowObserver.h"

// get a logger
static log4cpp::Category& logger( log4cpp::Category::getInstance( "Ubitrack.Facade.BasicFacade" ) );

namespace Ubitrack {
    namespace Facade {

        void initUbitrackLogging(basic_facade_string_type& filename)
        {
            Util::initLogging(filename.c_str());
        }


        BasicFacadePrivate::BasicFacadePrivate( basic_facade_string_type& sComponentPath )
                : AdvancedFacade( sComponentPath )
                , m_pBasicObserver( 0 )
        {}

        // translate from DataflowObserver to BasicDataflowObserver
        void BasicFacadePrivate::notifyAddComponent( basic_facade_string_type& sPatternName, basic_facade_string_type& sComponentName, const Graph::UTQLSubgraph& )
        {
            if ( m_pBasicObserver )
                m_pBasicObserver->notifyAddComponent( sPatternName.c_str(), sComponentName.c_str() );
        }

        void BasicFacadePrivate::notifyDeleteComponent( basic_facade_string_type& sPatternName, basic_facade_string_type& sComponentName )
        {
            if ( m_pBasicObserver )
                m_pBasicObserver->notifyDeleteComponent( sPatternName.c_str(), sComponentName.c_str() );
        }


        BasicFacade::BasicFacade( basic_facade_string_type& sComponentPath ) throw()
                : m_pPrivate( 0 )
                , m_sError( 0 )
        {
            try
            {
                m_pPrivate = new BasicFacadePrivate( sComponentPath );
                LOG4CPP_TRACE( logger, "BasicFacadePrivate created successfully" );
            }
            catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception constructing BasicFacade: " << e );
                setError( e.what() );
            }
        }


        BasicFacade::~BasicFacade()
        {
            delete m_pPrivate;
            if ( m_sError )
                free( m_sError );
        }

        unsigned long long int BasicFacade::now()
        {
            return Measurement::now();
        }


        bool BasicFacade::loadDataflow( basic_facade_string_type& sDfSrg ) throw()
        {
            try
            {
                m_pPrivate->loadDataflow( sDfSrg );
            }
            catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception in BasicFacade::loadDataflow: " << e );
                setError( e.what() );
                return false;
            }

            return true;
        }


        bool BasicFacade::loadDataflowString( basic_facade_string_type& sDataflow ) throw()
        {
            try
            {
                std::istringstream ss( sDataflow );
                m_pPrivate->loadDataflow( ss );
            }
            catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception in BasicFacade::loadDataflowString: " << e );
                setError( e.what() );
                return false;
            }

            return true;
        }


        void BasicFacade::clearDataflow() throw()
        {
            try
            {
                // start the event queue
                m_pPrivate->clearDataflow();
            }
            catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception in BasicFacade::clearDataflow: " << e );
                setError( e.what() );
            }
        }


        void BasicFacade::startDataflow() throw()
        {
            try
            {
                // start the event queue
                m_pPrivate->startDataflow();
            }
            catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception in BasicFacade::startDataflow: " << e );
                setError( e.what() );
            }
        }


        void BasicFacade::stopDataflow() throw()
        {
            try
            {
                // stop the event queue
                m_pPrivate->stopDataflow();
            }
            catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception in BasicFacade::stopDataflow: " << e );
                setError( e.what() );
            }
        }


        void BasicFacade::connectToServer( basic_facade_string_type& sAddress ) throw()
        {
            try
            {
                m_pPrivate->connectToServer( sAddress );
            }
            catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception in BasicFacade::connectToServer: " << e );
                setError( e.what() );
            }
        }


        void BasicFacade::sendUtqlToServer( basic_facade_string_type& sUtqlFile ) throw()
        {
            try
            {
                m_pPrivate->sendUtqlToServer( sUtqlFile );
            }
            catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception in BasicFacade::sendUtqlToServer: " << e );
                setError( e.what() );
            }
        }


        void BasicFacade::sendUtqlToServerString( basic_facade_string_type& buffer ) throw()
        {
            try
            {
                m_pPrivate->sendUtqlToServerString( buffer );
            }
            catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception in BasicFacade::sendUtqlToServerString: " << e );
                setError( e.what() );
            }
        }


        void BasicFacade::setDataflowObserver( BasicDataflowObserver* pObserver ) throw()
        {
            if ( !m_pPrivate->m_pBasicObserver )
                m_pPrivate->addDataflowObserver( m_pPrivate );
            m_pPrivate->m_pBasicObserver = pObserver;
        }


        void BasicFacade::removeDataflowObserver() throw()
        {
            m_pPrivate->removeDataflowObserver( m_pPrivate );
            m_pPrivate->m_pBasicObserver = 0;
        }


        const char* BasicFacade::getLastError() throw()
        {
            if ( m_pPrivate )
                return m_sError;
            else
                return "Error constructing BasicFacade -- see logging for details";
        }


        void BasicFacade::setError( const char* sMsg ) throw()
        {
            if ( m_sError )
                free( m_sError );
            m_sError = strdup( sMsg );
        }

        void BasicFacade::killEverything(){
            m_pPrivate->killEverything();
        }

        /** returns a wrapper for an ApplicationPullSink with a type defined via BMT **/
        template< typename BMT >
        std::shared_ptr<BasicPullSink< BMT > > BasicFacade::getPullSink( basic_facade_string_type& sName ) throw() {
            std::shared_ptr<BasicPullSink< BMT > > sink;
            try {
                sink = std::make_shared<BasicPullSink< BMT > >(sName, m_pPrivate);
            } catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception in BasicFacade::getPullSink: " << e );
                setError( e.what() );
            }
            return sink;
        }

        /** returns a wrapper for an ApplicationPushSink with a type defined via BMT **/
        template< typename BMT >
        std::shared_ptr<BasicPushSink< BMT > > BasicFacade::getPushSink( basic_facade_string_type& sName ) throw() {
            std::shared_ptr<BasicPushSink< BMT > > sink;
            try {
                sink = std::make_shared<BasicPushSink< BMT > >(sName, m_pPrivate);
            } catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception in BasicFacade::getPushSink: " << e );
                setError( e.what() );
            }
            return sink;
        }

        /** returns a wrapper for an ApplicationPullsource with a type defined via BMT **/
        template< typename BMT >
        std::shared_ptr<BasicPullSource< BMT > > BasicFacade::getPullSource( basic_facade_string_type& sName ) throw() {
            std::shared_ptr<BasicPullSource< BMT > > source = NULL;
            try {
                source = std::make_shared<BasicPullSource< BMT > >(sName, m_pPrivate);
            } catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception in BasicFacade::getPullSource: " << e );
                setError( e.what() );
            }
            return source;
        }

        /** returns a wrapper for an ApplicationPushSource with a type defined via BMT **/
        template< typename BMT >
        std::shared_ptr<BasicPushSource< BMT > > BasicFacade::getPushSource( basic_facade_string_type& sName ) throw() {
            std::shared_ptr<BasicPushSource< BMT > > source = NULL;
            try {
                source = std::make_shared<BasicPushSource< BMT > >(sName, m_pPrivate);
            } catch ( const Ubitrack::Util::Exception& e )
            {
                LOG4CPP_ERROR( logger, "Caught exception in BasicFacade::getPushSource: " << e );
                setError( e.what() );
            }
            return source;
        }


//        template< typename BMT >
//        class MeasurementExposer : public BasicFacade {
//        public:
//            /* does not work, since MeasurementExposer<BMT> is not a friend of BasicFacade */
//            template BasicPullSink  < BMT >* BasicFacade::getPullSink  <BasicScalarIntMeasurement>(basic_facade_string_type&);
//            template BasicPushSink  < BMT >* BasicFacade::getPushSink  <BasicScalarIntMeasurement>(basic_facade_string_type&);
//            template BasicPullSource< BMT >* BasicFacade::getPullSource<BasicScalarIntMeasurement>(basic_facade_string_type&);
//            template BasicPushSource< BMT >* BasicFacade::getPushSource<BasicScalarIntMeasurement>(basic_facade_string_type&);
//
//            /* this variant only exposes MeasurementExposer::getPu... but not BasicFacade::getPu... */
//            void implement_sinks(){
//
//                basic_facade_string_type name = "";
//                BasicPullSink  < BMT >* p1 = getPullSink  <BMT>(name);
//                BasicPushSink  < BMT >* p2 = getPushSink  <BMT>(name);
//                BasicPullSource< BMT >* p3 = getPullSource<BMT>(name);
//                BasicPushSource< BMT >* p4 = getPushSource<BMT>(name);
//
//                free(p1);
//                free(p2);
//                free(p3);
//                free(p4);
//            }
//        };

        
    }
}

//template class Ubitrack::Facade::MeasurementExposer< Ubitrack::Facade::BasicScalarIntMeasurement >;
//template class Ubitrack::Facade::MeasurementExposer< Ubitrack::Facade::BasicScalarDoubleMeasurement >;
//template class Ubitrack::Facade::MeasurementExposer< Ubitrack::Facade::BasicVector2Measurement >;
//template class Ubitrack::Facade::MeasurementExposer< Ubitrack::Facade::BasicMatrix33Measurement >;
//template class Ubitrack::Facade::MeasurementExposer< Ubitrack::Facade::BasicPoseMeasurement >;



/*
* can this be automated somehow ??????
* templates of templates ?? .. tried some options and failed, see above ..
*/
template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicScalarIntMeasurement > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicScalarIntMeasurement > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicScalarIntMeasurement > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicScalarIntMeasurement > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicScalarDoubleMeasurement > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicScalarDoubleMeasurement > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicScalarDoubleMeasurement > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicScalarDoubleMeasurement > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicVectorMeasurement< 2 > > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicVectorMeasurement< 2 > > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicVectorMeasurement< 2 > > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicVectorMeasurement< 2 > > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicVectorMeasurement< 3 > > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicVectorMeasurement< 3 > > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicVectorMeasurement< 3 > > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicVectorMeasurement< 3 > > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicVectorMeasurement< 4 > > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicVectorMeasurement< 4 > > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicVectorMeasurement< 4 > > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicVectorMeasurement< 4 > > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicVectorMeasurement< 8 > > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicVectorMeasurement< 8 > > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicVectorMeasurement< 8 > > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicVectorMeasurement< 8 > > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicMatrixMeasurement< 3, 3 > > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicMatrixMeasurement< 3, 3 > > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicMatrixMeasurement< 3, 3 > > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicMatrixMeasurement< 3, 3 > > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicMatrixMeasurement< 3, 4 > > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicMatrixMeasurement< 3, 4 > > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicMatrixMeasurement< 3, 4 > > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicMatrixMeasurement< 3, 4 > > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicMatrixMeasurement< 4, 4 > > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicMatrixMeasurement< 4, 4 > > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicMatrixMeasurement< 4, 4 > > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicMatrixMeasurement< 4, 4 > > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicPoseMeasurement > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicPoseMeasurement > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicPoseMeasurement > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicPoseMeasurement > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicRotationMeasurement > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicRotationMeasurement > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicRotationMeasurement > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicRotationMeasurement > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicErrorVectorMeasurement< 2 > > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicErrorVectorMeasurement< 2 > > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicErrorVectorMeasurement< 2 > > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicErrorVectorMeasurement< 2 > > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicErrorVectorMeasurement< 3 > > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicErrorVectorMeasurement< 3 > > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicErrorVectorMeasurement< 3 > > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicErrorVectorMeasurement< 3 > > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

template std::shared_ptr<Ubitrack::Facade::BasicPullSink  < Ubitrack::Facade::BasicImageMeasurement > > Ubitrack::Facade::BasicFacade::getPullSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSink  < Ubitrack::Facade::BasicImageMeasurement > > Ubitrack::Facade::BasicFacade::getPushSink  (basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPullSource< Ubitrack::Facade::BasicImageMeasurement > > Ubitrack::Facade::BasicFacade::getPullSource(basic_facade_string_type&);
template std::shared_ptr<Ubitrack::Facade::BasicPushSource< Ubitrack::Facade::BasicImageMeasurement > > Ubitrack::Facade::BasicFacade::getPushSource(basic_facade_string_type&);

#endif // ENABLE_BASICFACADE