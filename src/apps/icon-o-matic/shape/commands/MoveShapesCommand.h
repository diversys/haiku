/*
 * Copyright 2006-2007, Haiku.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Stephan Aßmus <superstippi@gmx.de>
 */
#ifndef MOVE_SHAPES_COMMAND_H
#define MOVE_SHAPES_COMMAND_H


#include "Command.h"


// TODO: make a templated "move items" command?

namespace BPrivate {
namespace Icon {
	class Shape;
	class ShapeContainer;
}
}
using namespace BPrivate::Icon;

class MoveShapesCommand : public Command {
 public:
								MoveShapesCommand(
									ShapeContainer* container,
									Shape** shapes,
									int32 count,
									int32 toIndex);
	virtual						~MoveShapesCommand();
	
	virtual	status_t			InitCheck();

	virtual	status_t			Perform();
	virtual status_t			Undo();

	virtual void				GetName(BString& name);

 private:
			ShapeContainer*		fContainer;
			Shape**				fShapes;
			int32*				fIndices;
			int32				fToIndex;
			int32				fCount;
};

#endif // MOVE_SHAPES_COMMAND_H
