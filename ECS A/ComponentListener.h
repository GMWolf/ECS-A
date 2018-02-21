#pragma once
namespace ecsa {
	class ComponentListener {

	public :
		virtual void entityAdded(int e) = 0;
		virtual void entityRemoved(int e) = 0;
	};
}