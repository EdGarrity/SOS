template <class base, class... params> class factory {
public:
  factory() {}
  factory(const factory &) = delete;
  factory &operator=(const factory &) = delete;

  auto create(const std::string name, params... args) {
    auto key = your_hash_func(name.c_str(), name.size());
    return std::move(create(key, args...));
  }

  auto create(key_t key, params... args) {
    std::unique_ptr<base> obj{creators_[key](args...)};
    return obj;
  }

  void register_creator(const std::string name,
                        std::function<base *(params...)> &&creator) {
    auto key = your_hash_func(name.c_str(), name.size());
    creators_[key] = std::move(creator);
  }

protected:
  std::unordered_map<key_t, std::function<base *(params...)>> creators_;
};


class base {
public:
  base(int val) : val_(val) {}

  virtual ~base() { std::cout << "base destroyed\n"; }

protected:
  int val_ = 0;
};

class foo : public base {
public:
  foo(int val) : base(val) { std::cout << "foo " << val << " \n"; }

  static foo *create(int val) { return new foo(val); }

  virtual ~foo() { std::cout << "foo destroyed\n"; }
};

class bar : public base {
public:
  bar(int val) : base(val) { std::cout << "bar " << val << "\n"; }

  static bar *create(int val) { return new bar(val); }

  virtual ~bar() { std::cout << "bar destroyed\n"; }
};

int main() {
  common::factory<base, int> factory;

  auto foo_creator = std::bind(&foo::create, std::placeholders::_1);
  auto bar_creator = std::bind(&bar::create, std::placeholders::_1);

  factory.register_creator("foo", foo_creator);
  factory.register_creator("bar", bar_creator);

  {
    auto foo_obj = std::move(factory.create("foo", 80));
    foo_obj.reset();
  }

  {
    auto bar_obj = std::move(factory.create("bar", 90));
    bar_obj.reset();
  }
}