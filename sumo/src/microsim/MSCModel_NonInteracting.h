/****************************************************************************/
/// @file    MSCModel_NonInteracting.h
/// @author  Melanie Weber
/// @author  Andreas Kendziorra
/// @date    Tue, 29 July 2014
/// @version $Id: MSCModel_NonInteracting.h 16290 2014-05-05 12:38:38Z namdre $
///
// The container following model for transfer (prototype)
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo-sim.org/
// Copyright (C) 2014-2014 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef MSCModel_NonInteracting_h
#define	MSCModel_NonInteracting_h

// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <string>
#include <limits>
#include <utils/common/SUMOTime.h>
#include <utils/common/Command.h>
#include <microsim/MSContainer.h>


// ===========================================================================
// class declarations
// ===========================================================================
class MSNet;
class MSLink;
class MSLane;
class MSJunction;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class MSCModel_NonInteracting
 * @brief The container following model for transfer
 *
 */
class MSCModel_NonInteracting {
public:

    /// @brief Constructor (it should not be necessary to construct more than one instance)
    MSCModel_NonInteracting(const OptionsCont& oc, MSNet* net);

    ~MSCModel_NonInteracting();

    // @brief walking directions
    static const int FORWARD;
    static const int BACKWARD;
    static const int UNDEFINED_DIRECTION;

    /// @brief the offset for computing container positions when being transfered
    static const SUMOReal LATERAL_OFFSET;

    class CState {
    public:
        CState() {};

        ~CState() {};

        /// @brief return the offset from the start of the current edge measured in its natural direction
        SUMOReal getEdgePos(const MSContainer::MSContainerStage_Transfer& stage, SUMOTime now) const;
        /// @brief return the network coordinate of the container
        Position getPosition(const MSContainer::MSContainerStage_Transfer& stage, SUMOTime now) const;
        /// @brief return the direction in which the container heading to
        SUMOReal getAngle(const MSContainer::MSContainerStage_Transfer& stage, SUMOTime now) const;
        /// @brief return the current speed of the container
        SUMOReal getSpeed(const MSContainer::MSContainerStage_Transfer& stage) const;
        /// @brief compute transfer time on edge and update state members
        SUMOTime computeTransferTime(const MSEdge* prev, const MSContainer::MSContainerStage_Transfer& stage, SUMOTime currentTime);


    private:
        SUMOTime myLastEntryTime;
        SUMOTime myCurrentDuration;
        SUMOReal myCurrentBeginPos;
        SUMOReal myCurrentEndPos;

    };

    /// @brief register the given person as a pedestrian
    CState* add(MSContainer* container, MSContainer::MSContainerStage_Transfer* stage, SUMOTime now);

    ///// @brief whether a pedestrian is blocking the crossing of lane at offset distToCrossing
    //bool blockedAtDist(const MSLane* lane, SUMOReal distToCrossing, std::vector<const MSPerson*>* collectBlockers);

private:
    class MoveToNextEdge : public Command {
    public:
        MoveToNextEdge(MSContainer* container, MSContainer::MSContainerStage_Transfer& transfer) : myParent(transfer), myContainer(container) {}
        ~MoveToNextEdge() {}
        SUMOTime execute(SUMOTime currentTime);

    private:
        MSContainer::MSContainerStage_Transfer& myParent;
        MSContainer* myContainer;
    private:
        /// @brief Invalidated assignment operator.
        MoveToNextEdge& operator=(const MoveToNextEdge&);
    };


private:
    /// @brief the net to which to issue moveToNextEdge commands
    MSNet* myNet;

};


#endif	/* MSCModel_NonInteracting_h */

