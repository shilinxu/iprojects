////////////////////////////////////////////////////////////////////////////////
//연필, 싸인펜, 붓의 기능을 담당한다.
//펜의 시작을 처리한다.                                  Modify by GSB(2003.2.1)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.MouseDown_Pen(Rec: TEventStruct_Ver20; X, Y: integer);
 var
   color: TColor32;
begin
  //DownPos := Point(X, Y);
  //MovePos := DownPos;    //이것은 마우스 다운에서 처리했다.

  //아래의 Point 배열(ptMPoint)은 BSpline 곡선을 그리기 위한 것이다.
  //Spline 곡선은 4개의 좌표를 요구한다.
  //4개의 좌표를 주면(0~3) 1~2 사이의 좌표에 대해 곡선을 그린다.
  // 0~1, 2~3의 좌표는 1~2사이의 곡선의 각도, 길이 등에 영향을 미친다.

  //마우스 다운일 때 3개의 좌표(0~2)에 같은 좌표의 값을 준다.
  //그러면 3개의 좌표를 들어올 때까지의 지연현상을 막을 수 있다.

  fDrawPenColor := Color32(Rec.EventColor);

  //if Rec.EventColor=clBlack then fDrawPenColor:= RGB( 10,10,10 )
  //else fDrawPenColor := Color32(Rec.EventColor);

  fDrawPenWidth := Rec.PenWidth;

  DownFirst:=True;
  BKfPressLineSize:=0.01;  //fPressLineSize;

  //연필 기능은 일정한 크기로 그리는 것이므로 이곳에서 BrushDot그리면 더 빠르게 쓸 수 있다.
  //////////////////////////////////////////////////////////////////////////////

  //if (gRecEvent_Ver20.EventStyle=esNormalPen_Ver20) or (gRecEvent_Ver20.EventStyle=esSignPen_Ver20) then

  //if Drawing then
  DrawPenDot( fDrawPenWidth/2 );   //연필, 싸인펜 가상 보여주기에 사용됨


  iLast_x  := -1;
  iLast_y  := -1;
  sgLast_x := -1;
  sgLast_y := -1;

  iMPointNum := 0;
  ptMPoint[iMPointNum].X := X;
  ptMPoint[iMPointNum].Y := Y;
  iMPointNum := iMPointNum+1;

  ptMPoint[iMPointNum].X := X;
  ptMPoint[iMPointNum].Y := Y;
  iMPointNum := iMPointNum+1;

  ptMPoint[iMPointNum].X := X;
  ptMPoint[iMPointNum].Y := Y;
  iMPointNum := iMPointNum+1;

end;

////////////////////////////////////////////////////////////////////////////////
//연필, 싸인펜, 붓의 기능을 담당한다.
//펜의 진행과정을 처리한다.                              Modify by GSB(2003.2.1)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.MouseMove_Pen(Rec:TEventStruct_Ver20; X, Y: Integer; DrawingSlideBuffer: Boolean);
 var
   dist: Integer;
begin

  if (iLast_x=X) and (iLast_y=Y) then exit;

  iLast_x:=X;
  iLast_y:=Y;

  ptMPoint[iMPointNum].X := X;
  ptMPoint[iMPointNum].Y := Y;

  dist := find_dist( MovePos.X, MovePos.Y, X, Y );  //두 좌표 사이의 거리를 Pixel로 return 한다.

  if dist < 3 then
  begin
    //if DrawingSlideBuffer then
    //DrawVirtualDot();
    exit;                                 //펜이나 마우스를 천천히 움직일 경우 Anti-Aliasing이 알고리즘 구조상으로 죽는다.
  end;                                    //그리고 마우스의 떨림으로 인한 쿼러티 저하를 막는 역활을 한다. 이것을 더 크게하면
                                          //질은 좋아지지만 마우스의 움직임과의 동질성 저하를 가져온다.
                                          //결론, 현재의 좌표가 앞의 좌표와의 거리가 3Pixel 이하이면 무시하고 빠져나간다.

  //if fPressLineSize > fDrawPenWidth then fPressLineSize:=fDrawPenWidth; //MessageDlg('Exiting ect Pascal application.', mtInformation,   [mbOk], 0);


  iMPointNum := iMPointNum+1;

  MovePos := Point(X, Y);

  //////////////////////////////////////////////////////////////////////////////

  DrawAllPen( Rec, False, DrawingSlideBuffer );

  //////////////////////////////////////////////////////////////////////////////

  iMPointNum := 0;

  ptMPoint[iMPointNum] := ptMPoint[1];   //아래의 Point 배열(ptMPoint)은 BSpline 곡선을 그리기 위한 것이다.
  iMPointNum := iMPointNum+1;            //Spline 곡선은 4개의 좌표를 요구한다.
                                         //4개의 좌표를 주면(0~3) 1~2 사이의 좌표에 대해 곡선을 그린다.
  ptMPoint[iMPointNum] := ptMPoint[2];   // 0~1, 2~3의 좌표는 1~2사이의 곡선의 각도, 길이 등에 영향을 미친다.
  iMPointNum := iMPointNum+1;

  ptMPoint[iMPointNum] := ptMPoint[3];
  iMPointNum := iMPointNum+1;

  DownFirst := False;
end;

////////////////////////////////////////////////////////////////////////////////
//연필, 싸인펜, 붓의 기능을 담당한다.
//펜의 끝을 처리한다.                                    Modify by GSB(2003.2.1)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.MouseUp_Pen(Rec:TEventStruct_Ver20; X, Y: Integer; DrawingSlideBuffer: Boolean);
 var
   bkPoint: TPoint;
begin
  MovePos := Point(X, Y);

  bkPoint := ptMPoint[2];

  ptMPoint[iMPointNum].X := X;
  ptMPoint[iMPointNum].Y := Y;
  iMPointNum := iMPointNum+1;

  //if fTabletDraw and ( bkPoint.X=X ) and ( bkPoint.Y=Y ) then
  DrawAllPen( Rec, True, DrawingSlideBuffer );
  //else
 // DrawAllPen( Rec, False, DrawingSlideBuffer );

  //////////////////////////////////////////////////////////////////////////////
end;


////////////////////////////////////////////////////////////////////////////////
//연필, 싸인펜, 붓의 기능을 담당한다.
//펜의 시작을 처리한다.                                  Modify by GSB(2003.2.1)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.MouseDown_LightPen(Rec: TEventStruct_Ver20; X, Y: Integer);
begin
  fDrawPenColor := Color32(Rec.EventColor);
  fDrawPenWidth := gPenWidth;

  DownFirst:=True;

  iLast_x  := -1;
  iLast_y  := -1;
  sgLast_x := -1;
  sgLast_y := -1;

  iMPointNum := 0;
  ptMPoint[iMPointNum].X := X;
  ptMPoint[iMPointNum].Y := Y;
  iMPointNum := iMPointNum+1;

  ptMPoint[iMPointNum].X := X;
  ptMPoint[iMPointNum].Y := Y;
  iMPointNum := iMPointNum+1;


  ptMPoint[iMPointNum].X := X;
  ptMPoint[iMPointNum].Y := Y;
  iMPointNum := iMPointNum+1;

end;

////////////////////////////////////////////////////////////////////////////////
//연필, 싸인펜, 붓의 기능을 담당한다.
//펜의 진행과정을 처리한다.                              Modify by GSB(2003.2.1)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.MouseMove_LightPen(Rec: TEventStruct_Ver20; X, Y: Integer; DrawingSlideBuffer: Boolean);
 var
  dist: Integer;
begin

  if (iLast_x=X) and (iLast_y=Y) then exit;

  iLast_x:=X;
  iLast_y:=Y;

  dist := find_dist( MovePos.X, MovePos.Y, X, Y );  //두 좌표 사이의 거리를 Pixel로 return 한다.

  if dist < 3 then exit;

  MovePos := Point(X, Y);

  ptMPoint[iMPointNum].X := X;
  ptMPoint[iMPointNum].Y := Y;
  iMPointNum := iMPointNum+1;

  //////////////////////////////////////////////////////////////////////////////

  DrawLightPen( Rec, DrawingSlideBuffer );

  //////////////////////////////////////////////////////////////////////////////

  iMPointNum := 0;

  ptMPoint[iMPointNum] := ptMPoint[1];   //아래의 Point 배열(ptMPoint)은 BSpline 곡선을 그리기 위한 것이다.
  iMPointNum := iMPointNum+1;            //Spline 곡선은 4개의 좌표를 요구한다.
                                         //4개의 좌표를 주면(0~3) 1~2 사이의 좌표에 대해 곡선을 그린다.
  ptMPoint[iMPointNum] := ptMPoint[2];   // 0~1, 2~3의 좌표는 1~2사이의 곡선의 각도, 길이 등에 영향을 미친다.
  iMPointNum := iMPointNum+1;

  ptMPoint[iMPointNum] := ptMPoint[3];
  iMPointNum := iMPointNum+1;

  DownFirst := False;
end;

////////////////////////////////////////////////////////////////////////////////
//연필, 싸인펜, 붓의 기능을 담당한다.
//펜의 끝을 처리한다.                                    Modify by GSB(2003.2.1)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.MouseUp_LightPen(Rec: TEventStruct_Ver20; X, Y: Integer; DrawingSlideBuffer: Boolean);
begin
  MovePos := Point(X, Y);

  ptMPoint[iMPointNum].X := X;
  ptMPoint[iMPointNum].Y := Y;
  iMPointNum := iMPointNum+1;

  //////////////////////////////////////////////////////////////////////////////

  DrawLightPen( Rec, DrawingSlideBuffer );

  //////////////////////////////////////////////////////////////////////////////
end;

//Integer용으로 사용하려면 Return 값을 받아 trunc 하기 바란다.
//BSPline에서 사용하는 것으로 가장 적합한 곡선의 x 좌표를 Return 한다.
function TfrmIPro.find_x( t:double; x10,x11,x12,x13:Integer ): Single;
begin
  Result := (((-x10+3*x11-3*x12+x13)*t*t*t+(3*x10-6*x11+3*x12)*t*t+(-3*x10+3*x12)*t+x10+4*x11+x12)/6); //+0.2; //000000000000008);
end;

//Integer용으로 사용하려면 Return 값을 받아 trunc 하기 바란다.
//BSPline에서 사용하는 것으로 가장 적합한 곡선의 y 좌표를 Return 한다.
function TfrmIPro.find_y( t:double; y10,y11,y12,y13:Integer ): Single;
begin
  Result := (((-y10+3*y11-3*y12+y13)*t*t*t+(3*y10-6*y11+3*y12)*t*t+(-3*y10+3*y12)*t+y10+4*y11+y12)/6);  //+0.2; //000000000000008);
end;

//2개의 좌표 거리를 Pixel로 반환한다.
function TfrmIPro.find_dist( x1, y1, x2, y2: Integer ): Integer;
begin
  Result := trunc( Sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) ) );
end;

procedure TfrmIPro.DrawPenBackDot( fLine: Single );
var
  Theta: Single;                   //Extended;  컴퓨터의 프로세스 따라 처리 능력이 차이가 난다.
  rDx1, rDy1: Single;              //Extended;  32Bit프로세스는 32Bit 변수를 가장 빠르게 동작한다.
begin                              //64Bit는 당연히 64Bit 변수를 사용하는 것이 좋다.
                                   //단, 개발툴도 지원해야 최적화 될 수 있다.
                                   //현재는 32Bit를 기준으로 했다. Modify by GSB(2003.2.1)
  fStartPoly.Clear;
  fStartPoly.Antialiased := True;

  fLine:=fLine+1;

  Theta := 0;
  while Theta < 3.1415926535 * 2 do    //8 do      //3.1415926535 * 2 do
  begin
    rDx1 := Cos(Theta);
    rDy1 := Sin(Theta);
    fStartPoly.Add( FixedPoint(rDx1*fLine, rDy1*fLine) );
    Theta := Theta + 0.2;
  end;

end;

procedure TfrmIPro.ImgSlideFlush(X1, Y1, X2, Y2: Integer);
var
  fCopyRect: TRect;
  fPenWidthgap: Integer;
begin
  // Update할 영역을 결정한다.
  fPenWidthgap := Round(fDrawPenWidth) + 1;
  if X1 < X2 then
  begin
    fCopyRect.Left := X1 - fPenWidthgap;
    fCopyRect.Right := X2 + fPenWidthgap;
  end else begin
    fCopyRect.Left := X2 - fPenWidthgap;
    fCopyRect.Right := X1 + fPenWidthgap;
  end;
  if Y1 < Y2 then
  begin
    fCopyRect.Top := Y1 - fPenWidthgap;
    fCopyRect.Bottom := Y2 + fPenWidthgap;
  end else begin
    fCopyRect.Top := Y2 - fPenWidthgap;
    fCopyRect.Bottom := Y1 + fPenWidthgap;
  end;
  if fCopyRect.Left < 0 then
    fCopyRect.Left := 0;
  if fCopyRect.Top < 0 then
    fCopyRect.Top := 0;
  if fCopyRect.Right >= ImgSlide.Width then
    fCopyRect.Right := ImgSlide.Width - 1;
  if fCopyRect.Bottom >= ImgSlide.Height then
    fCopyRect.Bottom := ImgSlide.Height - 1;

  ImgSlide.Flush(fCopyRect);
  Exit;
end;


procedure TfrmIPro.DrawAllPen(Rec: TEventStruct_Ver20; brusnEnd: Boolean; DrawingSlideBuffer: Boolean);
begin
  case Rec.EventStyle of
      esBrushPen_Ver20 :
      begin
        //if brusnEnd then DrawBrushPenEnd()    //붓 Bspline 곡선을 그린다.
        //else DrawBrushPen();                  //붓 Bspline 곡선의 원리로 그린다.

        if brusnEnd then DrawBrushPenEnd()      //붓 Bspline 곡선을 그린다.
        else DrawBrushPen();                    //붓 Bspline 곡선의 원리로 그린다.
      end;
      esSignPen_Ver20  : DrawSignPen();       //싸인펜
      esNormalPen_Ver20: DrawPencil();        //연필
  end;

  if DrawingSlideBuffer then
    UpdateCanvas(pbSlide[gCurrLoadedSlideIndex], rectTrick.Left, rectTrick.Top,
    rectTrick.Right, rectTrick.Bottom );
end;


////////////////////////////////////////////////////////////////////////////////
//Draw Virtual Dot -
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.DrawVirtualDot();
 var
    t, new_x, new_y: Single;
    color: TColor32;
begin
   /////////////////////// ImgSlide에 찍을 원을 그린다 /////////////////////////

   Color := SetAlpha( fDrawPenColor,95-trunc((fPressLineSize-1)*15) ); //색깔의 농도를 다른 것보다 흐리게 하였다.

   t:=0;
   while t < 1 do
   begin
     new_x := find_x( t, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
     new_y := find_y( t, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );
     SetPenDot( ImgSlide.Buffer, new_x, new_y, color );
     t := t + 0.494;
   end;

   ImgSlideFlush( ptMPoint[1].X, ptMPoint[1].Y, ptMPoint[2].X, ptMPoint[2].Y );
end;

procedure TfrmIPro.DrawPenDot( fLine: Single );
var
  Theta: Single;                   //Extended;  컴퓨터의 프로세스 따라 처리 능력이 차이가 난다.
  TmpPoly: TPolygon32;
  rDx1, rDy1: Single;              //Extended;  32Bit프로세스는 32Bit 변수를 가장 빠르게 동작한다.
begin                              //64Bit는 당연히 64Bit 변수를 사용하는 것이 좋다.
                                   //단, 개발툴도 지원해야 최적화 될 수 있다.
                                   //현재는 32Bit를 기준으로 했다. Modify by GSB(2003.2.1)
  fPolygon.Clear;
  fPolygon.Antialiased := True;
  fPolygon.FillMode := pfWinding;    //pfAlternate;  //

  //fEndPoly.Clear;
  //fEndPoly.Antialiased := True;
  //fEndPoly.FillMode := pfWinding;    //pfAlternate;  //

  Theta := 0;
  while Theta < 3.1415926535 * 2 do   //8 do      //3.1415926535 * 2 do
  begin
    rDx1 := Cos(Theta);
    rDy1 := Sin(Theta);
    fPolygon.Add( FixedPoint(rDx1*fLine, rDy1*fLine) );
    //fEndPoly.Add( FixedPoint(rDx1*(fLine+1), rDy1*(fLine+1) ) );
    Theta := Theta + 0.4;
  end;

  {
  fOutline.Free;
  fOutline := nil;

  TmpPoly := fPolygon.Outline;
  fOutline := TmpPoly.Grow(Fixed(fDrawPenWidth), 0.5);
  fOutline.FillMode := pfAlternate;  //pfWinding;
  TmpPoly.Free;
  fOutline.Antialiased := True;
  }

end;

procedure TfrmIPro.SetPenDot( ACanvas: TBitmap32; X, Y: Single; color: TColor32);
  var
    vh: Single;
begin
  //if ( sgLast_x=X ) and ( sgLast_y=Y ) then exit;
  //if ( abs(sgLast_x-X)< (fPressLineSize/(18-fDrawPenWidth) ) ) and ( abs(sgLast_y-Y)< (fPressLineSize/(18-fDrawPenWidth)) ) then exit;

  if Drawing then vh := fDrawPenWidth/(18-fDrawPenWidth)
  else vh := fPressLineSize/(18-fDrawPenWidth);

  if ( abs(sgLast_x-X)< vh ) and ( abs(sgLast_y-Y)< vh ) then exit;
  sgLast_x:=X; sgLast_y:=Y;


  fPolygon.Offset( Fixed(X-1), Fixed(Y-1) );
  fPolygon.DrawEdge( ACanvas, SetAlpha( fDrawPenColor, 6+fDrawPenWidth*2 ) );  //6+fDrawPenWidth*2 )  );

  fPolygon.Offset( Fixed(1), Fixed(1) );
  fPolygon.Draw( ACanvas, SetAlpha( fDrawPenColor, 6+fDrawPenWidth*2 ), color   );             //SetAlpha( fDrawPenColor, 30 )

  fPolygon.Offset( Fixed(X*-1), Fixed(Y*-1) );

  {
  fPolygon.Offset( Fixed(X), Fixed(Y) );
  fPolygon.DrawFill( ACanvas, fDrawPenColor );     //  ( fDrawPenColor, 2+fDrawPenWidth*4 )  );
  fPolygon.Offset( Fixed(X*-1), Fixed(Y*-1) );

  fEndPoly.Offset( Fixed(X), Fixed(Y) );
  fEndPoly.DrawEdge( ACanvas, SetAlpha(fDrawPenColor,9+fDrawPenWidth*3) );
  fEndPoly.Offset( Fixed((X)*-1), Fixed((Y)*-1) );
  }
end;

////////////////////////////////////////////////////////////////////////////////
//붓기능 - 펜 압력에 따라 굵기가 변한다(레벨은 1~7단계)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.DrawBrushPen();
 var
    t, inc, x, y: Single;    //, new_x, new_y
    dist: Integer;
    color: TColor32;
begin
   //Spline으로 그릴 원을 그린다.  //pbSlide[gCurrLoadedFileIndex].DrawMode := dmBlend;    //dmOpaque;
   x     := find_x( 0,    ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   y     := find_y( 0,    ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );
   new_x := find_x( 0.99, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   new_y := find_y( 0.99, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

   dist := find_dist( trunc(x), trunc(y), trunc(new_x), trunc(new_y) );

   if dist<1 then exit;
   if dist>100 then dist:=100;

   //inc:=1/(dist+fDrawPenWidth*3); //(distt/half ); ///(fPressLineSize*fPressLineSize); //1/(dist+(50/dist));
   inc:= 1/(dist+1);     //1/(dist+fDrawPenWidth*2);
   t:=inc;

   //if DownFirst then
   t:=0;

   /////////////////////// ImgSlide에 찍을 원을 그린다.

   ///if DownFirst then color := SetAlpha( fDrawPenColor, 30-(trunc(dist div 5)+(fDrawPenWidth*9)) )
   ///else color := SetAlpha( fDrawPenColor,110+(dist div 3)-trunc((fPressLineSize-2)*18) );                 //nc(fPressLineSize*-5) );   //SetAlpha( fDrawPenColor, round( (dist*0.32+955)/((fDrawPenWidth*8)+dist*4) ) ); //SetAlpha( fDrawPenColor, trunc( (dist*0.32+100)/(fDrawPenWidth) ));  // round( (dist*0.32+100)/(fDrawPenWidth) )  SetAlpha( fDrawPenColor,95-fDrawPenWidth );   //95-fDrawPenWidth);

   {
   if DownFirst then color := SetAlpha( fDrawPenColor, 50 )  //30-(trunc(dist div 5)+(fDrawPenWidth*9)) )
   else
   begin
         if (110-dist) < 50 then color := SetAlpha( fDrawPenColor, 50 )  ///110+(dist div 3)-trunc((fPressLineSize-2)*18) );
         else color := SetAlpha( fDrawPenColor, 110-dist );
   end;
   }

   if DownFirst then color := SetAlpha( fDrawPenColor, 80 )  //30-(trunc(dist div 5)+(fDrawPenWidth*9)) )
   else
   begin
     if (205-dist) < 90 then color := SetAlpha( fDrawPenColor, 90 )  ///110+(dist div 3)-trunc((fPressLineSize-2)*18) );
     else color := SetAlpha( fDrawPenColor, 205-dist );
   end;

   //color := SetAlpha( fDrawPenColor,100-trunc((fPressLineSize-1)*17) );

   if not Drawing then
     begin
       BKfPressLineSize := fPressLineSize;
       DrawPenDot( fPressLineSize/2 );
     end;
   // then

   ///////////////////////

   while t < 1 do
   begin
     new_x := find_x( t, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
     new_y := find_y( t, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );
     //SetPenDot( pbSlide[gCurrLoadedSlideIndex], new_x, new_y, color );
     SetPenDot( ImgSlide.Buffer, new_x, new_y, color );
     t := t + inc;
   end;

  rectTrick.Left   := trunc(x);
  rectTrick.Top    := trunc(y);
  rectTrick.Right  := trunc(new_x);
  rectTrick.Bottom := trunc(new_y);
end;

////////////////////////////////////////////////////////////////////////////////
//붓기능의 하나인 마지막 획의 끝부분을 뾰족하게 하는 루틴
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.DrawBrushPenEnd();
 var
    t, inc, x, y: Single;
    penwidth, ii, iinc:Extended;
    dist: Integer;
    color: TColor32;
begin
   x     := find_x( 0,    ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   y     := find_y( 0,    ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );
   new_x := find_x( 0.99, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   new_y := find_y( 0.99, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

   dist := find_dist( trunc(x), trunc(y), trunc(new_x), trunc(new_y) );

   if dist<1 then exit;                                                         //혹 upgrade하면서 실수로 다운을 일으키는 것을 대비하여 ...
   if dist>100 then dist:=100;

   inc:= 1/(dist+1);  //1/(dist+fDrawPenWidth*3);  //1/(dist/fPressLineSize );  //inc:=dist/(fPressLineSize*fPressLineSize);  //0.339; //1/(dist+(50/dist));
   t:=inc;

   //if DownFirst then
   t:=0;

   penwidth := BKfPressLineSize;  //fPressLineSize;
   //color := SetAlpha( fDrawPenColor, 100 );  //SetAlpha( fDrawPenColor,110+(dist div 3)-trunc((fPressLineSize-2)*18) );

   if DownFirst then color := SetAlpha( fDrawPenColor, 80 )  //30-(trunc(dist div 5)+(fDrawPenWidth*9)) )
   else
   begin
     if (205-dist) < 90 then color := SetAlpha( fDrawPenColor, 90 )  ///110+(dist div 3)-trunc((fPressLineSize-2)*18) );
     else color := SetAlpha( fDrawPenColor, 205-dist );
   end;

   //   ii := penwidth/inc;        //(penwidth-0.5)
   ii := (penwidth)/(1/inc);  //(penwidth-0.5)/(1/inc);        //(penwidth-0.5)
   
   
   while t < 1 do
   begin

     /////////////   원을 실시간 가변적으로 그리기 위해 While 문 안에 둔다. 물론 부하가 조금 걸린다.

     //if penwidth<0 then penwidth:=0;
     if penwidth<0.01 then penwidth:=0.01;  //0.2

     /////////////////////////////////// DrawDott에서 그린 원을 찍는다.

//     color := SetAlpha( fDrawPenColor, 100-trunc((fPressLineSize-1)*15) ); //120-trunc(fPressLineSize*-5) );   //-fDrawPenWidth ); //trunc(150-penwidth)); //-fDrawPenWidth);

     //penwidth:=penwidth-ii;    //0.1;  //25;  //0.3

     if not Drawing then DrawPenDot( penwidth/2 );



     new_x := find_x( t, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
     new_y := find_y( t, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

     SetPenDot( ImgSlide.Buffer, new_x, new_y, color );

     penwidth:=penwidth-ii;    //0.1;  //25;  //0.3

     ///////////////////////////////////

     t := t + inc;
   end;

  rectTrick.Left   := trunc(x);
  rectTrick.Top    := trunc(y);
  rectTrick.Right  := trunc(new_x);
  rectTrick.Bottom := trunc(new_y);
end;

////////////////////////////////////////////////////////////////////////////////
//싸인펜 - 펜 압력과 속도에 따라 색의 농도가 변한다(레벨은 1~7단계)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.DrawSignPen();
 var
    t, inc, x, y : Single;
    dist: Integer;
    color: TColor32;
begin                                                                                //Spline으로 그릴 원을 그린다.  //pbSlide[gCurrLoadedFileIndex].DrawMode := dmBlend;    //dmOpaque;
   x     := find_x( 0, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   y     := find_y( 0, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );
   new_x := find_x( 0.99, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   new_y := find_y( 0.99, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

   dist := find_dist( trunc(x), trunc(y), trunc(new_x), trunc(new_y) );

   if dist<1 then exit;                          //혹 upgrade하면서 실수로 다운을 일으키는 것을 대비하여 ...
   if dist>100 then dist:=100;

   if DownFirst then color := SetAlpha( fDrawPenColor, 40-dist ) //trunc(dist div 3) )  //+(fDrawPenWidth*9) )  )
   else
     begin
       if dist*3>55 then  color := SetAlpha( fDrawPenColor, 35 )
       else color := SetAlpha( fDrawPenColor, 90-trunc(dist*3) );  //trunc(dist div 3) );  //+(fDrawPenWidth*9)) );
     end;

   inc:=1/dist; //(dist+1+fDrawPenWidth*2);  //1/dist;1/(dist+fDrawPenWidth*3);  //1/dist;

   t:=inc;

   t:=0;

   /////////////////////// ImgSlide에 찍을 원을 그린다 -> MouseDown_Pen에서 그린다.

   while t < 1 do
   begin
     new_x := find_x( t, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
     new_y := find_y( t, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

     ///////////////////////////////////  DrawPenDot에서 그린 것을 ImgSlide에 찍는다.

     //if not DownFirst then
     SetPenDot( ImgSlide.Buffer, new_x, new_y, color );

     ///////////////////////////////////

     t := t + inc;
   end;

  rectTrick.Left   := trunc(x);
  rectTrick.Top    := trunc(y);
  rectTrick.Right  := trunc(new_x);
  rectTrick.Bottom := trunc(new_y);
end;

////////////////////////////////////////////////////////////////////////////////
//연필 - 일정한 굵기로 그린다(레벨은 1~7단계)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.DrawPencil();
 var
    t, inc, x, y: Single;
    dist: Integer;
    color: TColor32;
begin
                                                                                //Spline으로 그릴 원을 그린다.  //pbSlide[gCurrLoadedFileIndex].DrawMode := dmBlend;    //dmOpaque;
   x     := find_x( 0, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   y     := find_y( 0, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );
   new_x := find_x( 0.99, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   new_y := find_y( 0.99, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

   dist := find_dist( trunc(x), trunc(y), trunc(new_x), trunc(new_y) );

   if dist<1 then exit;                                                        //혹 upgrade하면서 실수로 다운을 일으키는 것을 대비하여 ...
   if dist>100 then dist:=100;

   //이 부분도 MouseDown_Pen 두면 좋겠지만 그렇게 하면 변수 선언... 좋지 못해 이곳에 둔다.

   //if DownFirst then color := SetAlpha( fDrawPenColor, 40 )  //color := SetAlpha( fDrawPenColor, 30-(trunc(dist div 5)+(fDrawPenWidth*9)) )
   //else color := SetAlpha( fDrawPenColor, 110 );  //color := SetAlpha(fDrawPenColor, 85+(dist div 2)-(fDrawPenWidth-2)*14 );   //color := SetAlpha( fDrawPenColor, 80+(dist div 4) ); //-trunc((fPressLineSize-2)*50) );

   {
   if DownFirst then color := SetAlpha( fDrawPenColor, 40 )  //color := SetAlpha( fDrawPenColor, 30-(trunc(dist div 5)+(fDrawPenWidth*9)) )
   else
   begin
         if (110-dist) < 80 then color := SetAlpha( fDrawPenColor, 80 )  ///110+(dist div 3)-trunc((fPressLineSize-2)*18) );
         else color := SetAlpha( fDrawPenColor, 110-dist );
   end;
   }

   if DownFirst then color := SetAlpha( fDrawPenColor, 80 )  //30-(trunc(dist div 5)+(fDrawPenWidth*9)) )
   else
   begin
     if (190-dist) < 90 then color := SetAlpha( fDrawPenColor, 90 )  ///110+(dist div 3)-trunc((fPressLineSize-2)*18) );
     else color := SetAlpha( fDrawPenColor, 190-dist );
   end;



   //inc:=1/(dist+fDrawPenWidth*2);
   inc:= 1/dist;

   t:=inc;

   //if DownFirst then
   t:=0;

   /////////////////////// ImgSlide에 찍을 원을 그린다 -> MouseDown_Pen에서 그린다.

   while t < 1 do
   begin
     new_x := find_x( t, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
     new_y := find_y( t, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

     ///////////////////////////////////  DrawPenDot에서 그린 것을 ImgSlide에 찍는다.

     //SetPenDot( pbSlide[gCurrLoadedSlideIndex], new_x, new_y, color );
     SetPenDot( ImgSlide.Buffer, new_x, new_y, color );

     ///////////////////////////////////

     t := t + inc;
   end;

   rectTrick.Left   := trunc(x);
   rectTrick.Top    := trunc(y);
   rectTrick.Right  := trunc(new_x);
   rectTrick.Bottom := trunc(new_y);
end;

procedure TfrmIPro.DrawLightPen( Rec: TEventStruct_Ver20; DrawingSlideBuffer: Boolean );
 var
    t, inc, x, y: Single;
    x1, y1, x2, y2, lheight, dist: Integer;
begin
   inc:=0.01;  //263;  //곡선을 그릴 경우 Pixel을 찍게 되는 1개의 곡선 내에 찍히는 1/0.01 점의 수가 된다(100개)
               //이 값을 0.점을 10개만 찍게 된다. 별로 예쁘지 않다. 대개 0.1을 사용한다.

   x     := find_x( 0, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   y     := find_y( 0, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );
   new_x := find_x( 0.99, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   new_y := find_y( 0.99, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

   dist := find_dist( trunc(x), trunc(y), trunc(new_x), trunc(new_y) );

   lheight := (Rec.PenWidth div 2)+2;

   t:=0;
   while t < 1 do
   begin
     new_x := find_x( t, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
     new_y := find_y( t, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

     if t <> 0 then  SetLightPenDot( Rec, ImgSlide.Buffer, new_x, new_y, dist );

     t := t + inc;
   end;

      if trunc(ptMPoint[1].X) < trunc(ptMPoint[2].X) then
        begin
          x1 := -8;
          x2 :=  8;
        end
      else
        begin
          x1 :=   8;
          x2 :=  -8;
        end;

      if trunc(ptMPoint[1].Y) < trunc(ptMPoint[2].Y) then
        begin
          y1 := -lheight;  //20
          y2 :=  lheight;
        end
      else
        begin
          y1 :=   lheight;
          y2 :=  -lheight;
        end;

      if DrawingSlideBuffer then UpdateCanvas(pbSlide[gCurrLoadedSlideIndex], ptMPoint[1].X+x1, ptMPoint[1].Y+y1, ptMPoint[2].X+x2, ptMPoint[2].Y+y2 );
end;

procedure TfrmIPro.SetLightPenDot( Rec: TEventStruct_Ver20; ACanvas: TBitmap32; x1, y1: Single; dist: Integer );
 var
  ret45: GRec45;
  fDstRect, fSrcRect: TRect;
  de: Integer;
  x, y: Integer;
begin
                                                    //fDrawPenColor
  ret45 := ACanvas.RetColorTS256( x1, y1, SetAlpha( Color32(clYellow), round( (dist*0.32+100)/(fDrawPenWidth) ) ) );

  x:= ret45.X;
  y:= ret45.Y;

  if (iLast_x=x) and (iLast_y=y) then exit;       //이것이 필요한 이유는 쓸데없이 같은 자리를 2번 찍을 것을 방지하는 것이다.
                                                  //짧은 거리를 100개의 점을 찍게 됨으로 속도와 퀄러티에 아주 중요하다.
  iLast_x := x;
  iLast_y := y;

  DrawLightPenDot( Rec, ACanvas, ret45.X,   ret45.Y,   ret45.per1 );
  DrawLightPenDot( Rec, ACanvas, ret45.X+1, ret45.Y,   ret45.per2 );
  DrawLightPenDot( Rec, ACanvas, ret45.X+1, ret45.Y+1, ret45.per3 );
  DrawLightPenDot( Rec, ACanvas, ret45.X,   ret45.Y+1, ret45.per4 );

end;

//색농도(d)를 사용해서 x,y 좌표에 점이나 도형을 찍는 함수
procedure TfrmIPro.DrawLightPenDot( Rec: TEventStruct_Ver20; ACanvas: TBitmap32; x, y, d: Integer );
 var
  color: TColor32;
begin
                                            //fDrawPenColor  gColor;
  color := SetAlpha( Color32(Rec.EventColor), 1 );  //Color32(clYellow), d div 12  );

  x:=x-7;
  y:=y-(Rec.PenWidth div 2);

  ACanvas.FillRectTS( x, y, x+14, y+Rec.PenWidth+1, color ); //사각
end;

procedure TfrmIPro.MouseDown_UnderLine(Button: TMouseButton; X, Y: Integer);
begin
  //bUpDown := False;
  //iDownUp := 0;
  //iUpDown := 0;
  //iUpDownLastX := X; //2147483647;       //4294967295/2 ->0xFFFFFFFF 4BYTE

  ImgSlide.Canvas.Pen.Mode := pmNotXor;
  ImgSlide.Canvas.Pen.Style := psDot;
end;

procedure TfrmIPro.MouseMove_UnderLine(X, Y: Integer);
  var
    distX: Integer;
begin
  ImgSlide.Canvas.Moveto(DownPos.X, DownPos.Y);
  ImgSlide.Canvas.LineTo(MovePos.X, DownPos.Y); //MovePos.Y);

  //////////////////////////////////////////////////////////////////////////////

  {distX := abs(iUpDownLastX-X);

  if  abs(DownPos.Y-Y) > 3 then
    begin

    if ((DownPos.X=MovePos.X) and (DownPos.Y=MovePos.Y)) or ( (bUpDown=False) and (MovePos.Y-Y > 0) ) then       //아래에서 위로 진행  DownUp,
      begin
        bUpDown := True;
        if distX < 120 then
          begin
            iDownUp := iDownUp + 1;
            iUpDownLastX := X;
          end;
      end
    else if ((DownPos.X=MovePos.X) and (DownPos.Y=MovePos.Y)) or ( (bUpDown=True) and (MovePos.Y-Y < 0) ) then   //위에서 아래로 진행  UpDown,
           begin
             bUpDown := False;
             if distX < 120 then
               begin
                 iUpDown := iUpDown + 1;
                 iUpDownLastX := X;
               end;
           end;
  end;

  //////////////////////////////////////////////////////////////////////////////
  }

  MovePos := Point(X, Y);
  ImgSlide.Canvas.Moveto(DownPos.X, DownPos.Y);
  ImgSlide.Canvas.LineTo(MovePos.X, DownPos.Y);
end;

procedure TfrmIPro.MouseUp_UnderLine(Event: TEventStruct_Ver20; X, Y: Integer; DrawingSlideBuffer: Boolean);
  var
    dist: Integer;
begin

  MovePos := Point(X, Y);

  if DrawingSlideBuffer then                                                    // Undo / Redo 작업시
  begin
    ImgSlide.Canvas.MoveTo(DownPos.X, DownPos.Y);
    ImgSlide.Canvas.LineTo(X, DownPos.Y);
  end;

  //////////////////////////////////////////////////////////////////////////////

  fDrawPenColor := Color32(Event.EventColor);
  fDrawPenWidth := Event.PenWidth;

  DrawUnderLine( ImgSlide.Buffer, DownPos.X, DownPos.Y + fDrawPenWidth, MovePos.X, MovePos.Y );

                         //dist/(iDownUp+iUpDown) > 4
  {if (iDownUp+iUpDown) > 3 then DrawUnderLine( ImgSlide.Buffer, DownPos.X, DownPos.Y + fDrawPenWidth, MovePos.X, MovePos.Y )
  else DrawLine(ImgSlide.Buffer, DownPos.X, DownPos.Y + round(fDrawPenWidth/2), MovePos.X, DownPos.Y + round(fDrawPenWidth/2));}

  if DrawingSlideBuffer then ImgSlide.Flush;

end;

procedure TfrmIPro.DrawUnderLine( ACanvas: TBitmap32; X1, Y1, X2, Y2: Integer );
  var
    i, iImsi: Integer;
    sgSignBase: Single;
begin
  DrawPenDot( fDrawPenWidth/2 );
  if X1 > X2 then
    begin
      iImsi := X2;
      X2 := X1;
      X1 := iImsi;
    end;
  i:=X1;

  iImsi := i;

  while i < X2 do   //300은 Pixel 길이
    begin           //X     Y         //2*PI    주파수                      //곡선의 높이
      SetPenDot( ACanvas, i+0.0, Y1-sin( (2*PI)/(22)*(118-fDrawPenWidth/5+(i-iImsi) ) )*(1.5+(fDrawPenWidth/15)), fDrawPenColor );
      i:=i+1;
    end;
end;

