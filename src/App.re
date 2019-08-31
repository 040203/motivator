module AppContainer = {
  [@react.component]
  let make = () => {
    let (profile, _) = Profile.useProfile();
    let (lastWeight, _) = LastWeight.useLastWeight();

    if (profile === None || lastWeight === None) {
      <Authorize />;
    } else {
      <AuthenticateInvariant> <TabNavigator /> </AuthenticateInvariant>;
    };
  };
};

let app = () => {
  let (migrated, setMigrated) = React.useState(_ => false);

  React.useEffect0(_ => {
    Db.migrate(_ => setMigrated(_ => true));
    None;
  });

  if (!migrated) {
    <StyledText value="loading..." />;
  } else {
    <Profile> <LastWeight> <AppContainer /> </LastWeight> </Profile>;
  };
};