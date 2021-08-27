#pragma once

namespace gn {
	class Object {
	public:
		virtual ~Object() {}

		virtual void Create() {}
	};
}
