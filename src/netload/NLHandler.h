/****************************************************************************/
/// @file    NLHandler.h
/// @author  Daniel Krajzewicz
/// @date    Sept 2002
/// @version $Id$
///
// The XML-Handler for network loading
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2011 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef NLHandler_h
#define NLHandler_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include <utils/common/SUMOTime.h>
#include <utils/xml/SUMOXMLDefinitions.h>
#include <microsim/MSLink.h>
#include <microsim/MSRouteHandler.h>
#include <microsim/traffic_lights/MSSimpleTrafficLightLogic.h>
#include <microsim/traffic_lights/MSActuatedTrafficLightLogic.h>
#include <microsim/MSBitSetLogic.h>
#include "NLBuilder.h"
#include "NLDiscreteEventBuilder.h"
#include "NLSucceedingLaneBuilder.h"


// ===========================================================================
// class declarations
// ===========================================================================
class NLContainer;
class NLDetectorBuilder;
class NLTriggerBuilder;
class MSTrafficLightLogic;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class NLNetHandler
 * @brief The XML-Handler for network loading
 *
 * The SAX2-handler responsible for parsing networks and routes to load.
 * This is an extension of the MSRouteHandler as routes and vehicles may also
 *  be loaded from network descriptions.
 */
class NLHandler : public MSRouteHandler {
public:
    /// @brief Definition of a lane vector
    typedef std::vector<MSLane*> LaneVector;

public:
    /** @brief Constructor
     *
     * @param[in] file Name of the parsed file
     * @param[in, out] net The network to fill
     * @param[in] detBuilder The detector builder to use
     * @param[in] triggerBuilder The trigger builder to use
     * @param[in] edgeBuilder The builder of edges to use
     * @param[in] junctionBuilder The builder of junctions to use
     */
    NLHandler(const std::string &file, MSNet &net,
              NLDetectorBuilder &detBuilder, NLTriggerBuilder &triggerBuilder,
              NLEdgeControlBuilder &edgeBuilder,
              NLJunctionControlBuilder &junctionBuilder) throw();


    /// @brief Destructor
    virtual ~NLHandler() throw();


protected:
    /// @name inherited from GenericSAXHandler
    //@{

    /** @brief Called on the opening of a tag;
     *
     * @param[in] element ID of the currently opened element
     * @param[in] attrs Attributes within the currently opened element
     * @exception ProcessError If something fails
     * @see GenericSAXHandler::myStartElement
     * @todo Refactor/describe
     */
    virtual void myStartElement(int element,
                                const SUMOSAXAttributes &attrs) throw(ProcessError);


    /** @brief Called when a closing tag occurs
     *
     * @param[in] element ID of the currently opened element
     * @exception ProcessError If something fails
     * @see GenericSAXHandler::myEndElement
     * @todo Refactor/describe
     */
    virtual void myEndElement(int element) throw(ProcessError);
    //@}


protected:
    void addParam(const SUMOSAXAttributes &attrs);

    /** @brief Builds an e1 detector using the given specification
     * @param[in] attrs The attributes that define the detector
     */
    virtual void addE1Detector(const SUMOSAXAttributes &attrs);

    /** @brief Builds an e2 detector using the given specification
     * @param[in] attrs The attributes that define the detector
     */
    virtual void addE2Detector(const SUMOSAXAttributes &attrs);

#ifdef _MESSAGES
    /** @brief Builds an e4-detector using the given specification
     * @param[in] attrs The attributes that define the detector
     */
    virtual void addMsgDetector(const SUMOSAXAttributes &attrs);
#endif

    /** @brief Starts building of an e3 detector using the given specification
     * @param[in] attrs The attributes that define the detector
     */
    void beginE3Detector(const SUMOSAXAttributes &attrs);

    /** @brief Adds an entry to the currently processed e3 detector
     * @param[in] attrs The attributes that define the entry
     */
    void addE3Entry(const SUMOSAXAttributes &attrs);

    /** @brief Adds an exit to the currently processed e3 detector
     * @param[in] attrs The attributes that define the exit
     */
    void addE3Exit(const SUMOSAXAttributes &attrs);

    /// Builds of an e3 detector using collected values
    virtual void endE3Detector();

    /** @brief Builds a vtype-detector using the given specification
     * @param[in] attrs The attributes that define the detector
     */
    virtual void addVTypeProbeDetector(const SUMOSAXAttributes &attrs);

    /** @brief Builds a routeprobe-detector using the given specification
     * @param[in] attrs The attributes that define the detector
     */
    virtual void addRouteProbeDetector(const SUMOSAXAttributes &attrs);

    /** @brief Builds edge or lane base mean data collector using the given specification
     * @param[in] attrs The attributes that define the detector
     * @param[in] objecttype The type (lane/edge) of the parsed mean data definition
     */
    virtual void addEdgeLaneMeanData(const SUMOSAXAttributes &attrs, int objecttype);

    /// Closes the process of building an edge
    virtual void closeEdge();


protected:
    /// The net to fill (preinitialised)
    MSNet &myNet;


private:
    /// begins the processing of an edge
    void beginEdgeParsing(const SUMOSAXAttributes &attrs);

    /// adds a lane to the previously opened edge
    void addLane(const SUMOSAXAttributes &attrs);

    /// adds a polygon
    void addPOI(const SUMOSAXAttributes &attrs);

    /// adds a polygon
    void addPoly(const SUMOSAXAttributes &attrs);

    /// (deprecated) adds a logic item to the current logic
    void addLogicItem(const SUMOSAXAttributes &attrs);

    ///  adds a request item to the current junction logic
    void addRequest(const SUMOSAXAttributes &attrs);

    /// begins the reading of a junction row logic
    void initJunctionLogic(const SUMOSAXAttributes &attrs);

    /// begins the reading of a traffic lights logic
    void initTrafficLightLogic(const SUMOSAXAttributes &attrs);

    /// adds a phase to the traffic lights logic currently build
    void addPhase(const SUMOSAXAttributes &attrs);


    /// opens a junction for processing
    virtual void openJunction(const SUMOSAXAttributes &attrs);

    void parseLanes(const std::string &junctionID, const std::string &def, std::vector<MSLane*> &into, bool &ok);

#ifdef _MESSAGES
    /// adds a message emitter
    void addMsgEmitter(const SUMOSAXAttributes &attrs);
#endif

    /// opens the container of succeeding lanes for processing (deprecated, see addConnection))
    void openSucc(const SUMOSAXAttributes &attrs);

    /// adds a succeeding lane (deprecated, see addConnection)
    void addSuccLane(const SUMOSAXAttributes &attrs);

    /// adds a connection
    void addConnection(const SUMOSAXAttributes &attrs);

    virtual void openWAUT(const SUMOSAXAttributes &attrs);
    void addWAUTSwitch(const SUMOSAXAttributes &attrs);
    void addWAUTJunction(const SUMOSAXAttributes &attrs);

    /// Parses network location description
    void setLocation(const SUMOSAXAttributes &attrs);

    /** @begin Parses a district and creates a pseudo edge for it
     *
     * Called on the occurence of a "district" element, this method
     *  retrieves the id of the district and creates a district type
     *  edge with this id.
     *
     * @param[in] attrs The attributes (of the "district"-element) to parse
     * @exception ProcessError If an edge given in district@edges is not known
     */
    void addDistrict(const SUMOSAXAttributes &attrs) throw(ProcessError);


    /** @begin Parses a district edge and connects it to the district
     *
     * Called on the occurence of a "dsource" or "dsink" element, this method
     *  retrieves the id of the approachable edge. If this edge is known
     *  and valid, the approaching edge is informed about it.
     *
     * @param[in] attrs The attributes to parse
     * @param[in] isSource whether a "dsource or a "dsink" was given
     * @todo No exception?
     */
    void addDistrictEdge(const SUMOSAXAttributes &attrs, bool isSource);


    void closeWAUT();

    /// closes the processing of a lane
    void closeSuccLane();

    /// @brief Parses the given character into an enumeration typed link direction
    LinkDirection parseLinkDir(const std::string& dir);

    /// @brief Parses the given character into an enumeration typed link state
    LinkState parseLinkState(const std::string& state);

    /// @brief retrieves <fromLane, toLane> based on the edges and an index specifier (i.e. '0:1')
    std::pair<MSLane*, MSLane*> getLanesFromIndices(MSEdge *from, MSEdge *to, const std::string &laneIndices, bool &ok);


protected:
    /// @brief A builder for object actions
    NLDiscreteEventBuilder myActionBuilder;

    /// @brief Information whether the currently parsed edge is internal and not wished, here
    bool myCurrentIsInternalToSkip;


    /// @brief The detector builder to use
    NLDetectorBuilder &myDetectorBuilder;

    /// @brief The trigger builder to use
    NLTriggerBuilder &myTriggerBuilder;

    /// @brief The edge builder to use
    NLEdgeControlBuilder &myEdgeControlBuilder;

    /// @brief The junction builder to use
    NLJunctionControlBuilder &myJunctionControlBuilder;

    /// @brief The lane-to-lane connections builder to use
    NLSucceedingLaneBuilder mySucceedingLaneBuilder;



    /// The id of the current district
    std::string myCurrentDistrictID;

    /// internal information whether a tls-logic is currently read
    bool myAmInTLLogicMode;

    /// The id of the currently processed WAUT
    std::string myCurrentWAUTID;

    /// The network offset
    Position myNetworkOffset;

    /// The network's boundaries
    Boundary myOrigBoundary, myConvBoundary;

    bool myCurrentIsBroken;

    bool myHaveWarnedAboutDeprecatedE1, myHaveWarnedAboutDeprecatedE2,
    myHaveWarnedAboutDeprecatedE3, myHaveWarnedAboutDeprecatedDetEntry,
    myHaveWarnedAboutDeprecatedDetExit, myHaveWarnedAboutDeprecatedTimedEvent;
    bool myHaveWarnedAboutDeprecatedRowLogic, myHaveWarnedAboutDeprecatedTLLogic;
    bool myHaveWarnedAboutDeprecatedTLSTiming;
    bool myHaveWarnedAboutDeprecatedTimeThreshold,
    myHaveWarnedAboutDeprecatedSpeedThreshold,
    myHaveWarnedAboutDeprecatedJamDistThreshold;
    bool myHaveWarnedAboutDeprecatedVTypeProbe, myHaveWarnedAboutDeprecatedRouteProbe,
    myHaveWarnedAboutDeprecatedEdgeMean, myHaveWarnedAboutDeprecatedLaneMean;
    bool myHaveWarnedAboutDeprecatedVTypes, myHaveWarnedAboutDeprecatedLanes;
    bool myHaveWarnedAboutDeprecatedDistrict, myHaveWarnedAboutDeprecatedDSource, myHaveWarnedAboutDeprecatedDSink;


private:
    /** invalid copy constructor */
    NLHandler(const NLHandler &s);

    /** invalid assignment operator */
    NLHandler &operator=(const NLHandler &s);

};


#endif

/****************************************************************************/

