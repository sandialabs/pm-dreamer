/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/src/MutationOp.cpp
 *  \brief  Source code of class LinGP::MutationOp.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.1 $
 *  $Date: 2005/10/06 15:43:44 $
 */

#include <algorithm>
#include <string>

#include "beagle/GP.hpp"
#include "beagle/LinGP/MutationOp.hpp"
#include "beagle/LinGP/Program.hpp"
#include "beagle/RunTimeException.hpp"
#include "beagle/ValidationException.hpp"
#include "beagle/LinGP/Context.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a linear GP mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inInstructMutatePbName Instruction mutation probability parameter name used in register.
 *  \param inName Name of the operator.
 */
LinGP::MutationOp::MutationOp(string inMutationPbName,
                              string inInstructMutatePbName,
                              string inName) :
  Beagle::MutationOp(inMutationPbName, inName),
  mInstructMutatePbName(inInstructMutatePbName)
{ }


/*!
 *  \brief Initialize the linear GP mutation operator.
 *  \param ioSystem System of the evolution.
 */
void LinGP::MutationOp::initialize(Beagle::System& ioSystem)
{
  Beagle::MutationOp::initialize(ioSystem);
  
  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    ioSystem.getRegister().deleteEntry(mMutationPbName);
  }
  
  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    mMutationProba = castHandleT<Float>(ioSystem.getRegister()[mMutationPbName]);
  } else {
    mMutationProba = new Float(float(1.0));
    Register::Description lProbaDescription(
      "Individual linear GP mutation prob.",
      "Float",
      "1.0",
      "Linear GP mutation probability for each individual."
    );
    ioSystem.getRegister().addEntry(mMutationPbName, mMutationProba, lProbaDescription);
  }

  if(ioSystem.getRegister().isRegistered(mInstructMutatePbName)) {
    mInstructMutateProba = castHandleT<Float>(ioSystem.getRegister()[mInstructMutatePbName]);
  } else {
    mInstructMutateProba = new Float(float(0.05));
    Register::Description lDescription(
      "Instruction mutation probability",
      "Float",
      "0.05",
      "Probability for each instruction inverted by mutation, when an individual is mutated."
    );
    ioSystem.getRegister().addEntry(mInstructMutatePbName, mInstructMutateProba, lDescription);
  }
}


/*!
 *  \brief Mutate a linear GP individual.
 *  \param ioIndividual Linear GP individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool LinGP::MutationOp::mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext)
{
  Beagle_ValidateParameterM(mInstructMutateProba->getWrappedValue()>=0.0,
                            mInstructMutatePbName, "<0");
  bool lMutated = false;
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::LinGP::MutationOp",
    string("Linear GP mutation probability is: ")+
    dbl2str(mInstructMutateProba->getWrappedValue())
  );

  LinGP::Context& lLinGPContext = castObjectT<LinGP::Context&>(ioContext);
  LinGP::InstructionSuperSet& lInsSS = lLinGPContext.getSystem().getInstructionSuperSet();

  for(unsigned int i=0; i<ioIndividual.size(); i++) {
    LinGP::Program::Handle lProgram = castHandleT<LinGP::Program>(ioIndividual[i]);
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "mutation", "Beagle::LinGP::MutationOp",
      string("Mutating the ")+uint2ordinal(i+1)+" program"
    );
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "mutation", "Beagle::LinGP::MutationOp",
      string("The program before mutation: ")+lProgram->serialize()
    );
    for(unsigned int j=0; j<lProgram->size(); j++) {
      double lRolledPb = ioContext.getSystem().getRandomizer().rollUniform();
      if(lRolledPb <= mInstructMutateProba->getWrappedValue()) {
        (*lProgram)[j] =
          lInsSS[i]->selectRandomInstruction(lLinGPContext.getSystem())->giveReference(lLinGPContext);
        lMutated = true;
      }
    }
    if(lMutated) {
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::LinGP::MutationOp",
        string("The program has been mutated")
      );
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::LinGP::MutationOp",
        string("The program after mutation: ")+lProgram->serialize()
      );
    }
    else {
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::LinGP::MutationOp",
        string("The program has not been mutated")
      );
    }
  }
  return lMutated;
}


/*!
 *  \brief Read a linear GP mutation operator from XML tree.
 *  \param inNode XML node to use to read mutation operator.
 *  \param inOpMap Operator map to use to read mutation operator.
 */
void LinGP::MutationOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != getName())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
  }
  string lMutationPbReadName = inIter->getAttribute("mutationpb");
  if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
  string lInstructMutatePbReadName = inIter->getAttribute("mutinstructpb");
  if(lInstructMutatePbReadName.empty() == false) mInstructMutatePbName = lInstructMutatePbReadName;
}


/*!
 *  \brief Write linear GP mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void LinGP::MutationOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle::MutationOp::writeContent(ioStreamer, inIndent);
  ioStreamer.insertAttribute("mutinstructpb", mInstructMutatePbName);
}

