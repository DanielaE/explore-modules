export module mod;
#define WORKAROUND
namespace detail {
	template <typename T>
	T incr(T c) {
		return ++c;
	}

	int more(int i) {
		return ++i;
	}

	template <typename T>
	struct C {
		C() = default;
		C(T c) : c_{ c } {}
		T c_;
	};

	template <typename T>
	struct context {
		context(C<T> c) : c_{ c } {}
		C<T> c_;
	};

	struct tag {};

	template <typename T>
	T bar(C<T> c) {
#ifdef WORKAROUND
		// symbolic links to aide name lookup
		using detail::context;
		using detail::C;
		using detail::incr;
		using detail::more;
		using detail::tag;
#endif
		struct local_temploid {
			context<T> ctx_;
			local_temploid(C<T> c) : ctx_(c) {}
			auto handle(tag) { return incr(ctx_.c_.c_); }
			auto handle() { return more(ctx_.c_.c_); }
		};
		local_temploid t{ c };
		return t.handle(tag()) + t.handle();
	}

	template <typename T>
	struct legit_template {
		context<T> ctx_;
		legit_template(C<T> c) : ctx_(c) {}
		auto handle(tag) { return incr(ctx_.c_.c_); }
		auto handle() { return more(ctx_.c_.c_); }
	};

	template <typename T>
	T baz(C<T> c) {
		legit_template<T> t{ c };
		return t.handle(tag()) + t.handle();
	}
}

export
template <typename T>
[[maybe_unused]] auto foo(T c) {
	detail::C<T> cfoo(c);
	auto a = bar(cfoo);
	auto b = baz(cfoo);
	return a+b;
}
