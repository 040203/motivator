[@react.component]
let app = () => {
  let (profile, _) = Profile.useProfile();

  React.useEffect0(_ => {
    Db.migrate(ignore);
    None;
  });

  if (profile === None) {
    <Authorize />
  } else {
    <AuthenticateInvariant> <TabNavigator /> </AuthenticateInvariant>;
  };
};