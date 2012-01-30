#ifndef GRIM_TMPRESOURCE_H
#define GRIM_TMPRESOURCE_H

#include "engines/grim/resource.h"

namespace Grim {

template<class T>
class SharedData {
public:
	static T *create(const Common::String &name) {
		if (!g_resourceloader->getFileExists(name)) {
			warning("Could not find file %s", name.c_str());
			return 0;
		}
		T *o = new T();
		o->_name = name;
		return o;
	}

	SharedData<T>() {
		_loaded = false;
	}
	
	virtual bool load(Common::SeekableReadStream *data) = 0;

	bool loadres() {
		Common::SeekableReadStream *stream = g_resourceloader->openNewStreamFile(_name);
		bool res = load(stream);
		delete stream;
		return res;
	}

	bool requireLoaded() {
		bool l = false;
		if (!_loaded) {
			if (_loaded = loadres()) {
				l = true;
			}
		}
		return l;
	}
	Common::String _name;
	bool _loaded;
};

template<class T, class D>
class LoadableResource {
public:
	LoadableResource(const Common::String &name);
	LoadableResource() {}

	static Common::HashMap<Common::String, D *> *_map;

	static T *create(const Common::String &name) {
		T *o = new T(name);
		if (!o->_data) {
			delete o;
			o = NULL;
		} else {
			if (o->_data->_loaded) {
				o->postLoad();
			}
		}
		return o;
	}

	const Common::String &getFilename() { return _name; }

	virtual void postLoad() { }

	void requireLoaded() const;

protected:
	Common::String _name;
	D *_data;
};

template <class T, class D>
Common::HashMap<Common::String, D *> *LoadableResource<T, D>::_map;

template <class T, class D>
LoadableResource<T, D>::LoadableResource(const Common::String &name) : _name(name) {
	_name.toLowercase();
	if (!_map) {
		_map = new Common::HashMap<Common::String, D *>();
	}

	Common::HashMap<Common::String, D *>::iterator it = _map->find(_name);
	if (it != _map->end()) {
		_data = it->_value;
	} else {
		_data = D::create(_name);
		if (_data) {
			_map->setVal(_name, _data);
		}
	}
}

template <class T, class D>
void LoadableResource<T, D>::requireLoaded() const {
	if (_data->requireLoaded()) {
		const_cast<LoadableResource<T, D> *>(this)->postLoad();
	}
}

}
#endif