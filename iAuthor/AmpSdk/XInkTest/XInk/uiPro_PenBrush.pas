////////////////////////////////////////////////////////////////////////////////
//����, ������, ���� ����� ����Ѵ�.
//���� ������ ó���Ѵ�.                                  Modify by GSB(2003.2.1)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.MouseDown_Pen(Rec: TEventStruct_Ver20; X, Y: integer);
 var
   color: TColor32;
begin
  //DownPos := Point(X, Y);
  //MovePos := DownPos;    //�̰��� ���콺 �ٿ�� ó���ߴ�.

  //�Ʒ��� Point �迭(ptMPoint)�� BSpline ��� �׸��� ���� ���̴�.
  //Spline ��� 4���� ��ǥ�� �䱸�Ѵ�.
  //4���� ��ǥ�� �ָ�(0~3) 1~2 ������ ��ǥ�� ���� ��� �׸���.
  // 0~1, 2~3�� ��ǥ�� 1~2������ ��� ����, ���� � ������ ��ģ��.

  //���콺 �ٿ��� �� 3���� ��ǥ(0~2)�� ���� ��ǥ�� ���� �ش�.
  //�׷��� 3���� ��ǥ�� ���� �������� ���������� ���� �� �ִ�.

  fDrawPenColor := Color32(Rec.EventColor);

  //if Rec.EventColor=clBlack then fDrawPenColor:= RGB( 10,10,10 )
  //else fDrawPenColor := Color32(Rec.EventColor);

  fDrawPenWidth := Rec.PenWidth;

  DownFirst:=True;
  BKfPressLineSize:=0.01;  //fPressLineSize;

  //���� ����� ������ ũ��� �׸��� ���̹Ƿ� �̰����� BrushDot�׸��� �� ������ �� �� �ִ�.
  //////////////////////////////////////////////////////////////////////////////

  //if (gRecEvent_Ver20.EventStyle=esNormalPen_Ver20) or (gRecEvent_Ver20.EventStyle=esSignPen_Ver20) then

  //if Drawing then
  DrawPenDot( fDrawPenWidth/2 );   //����, ������ ���� �����ֱ⿡ ����


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
//����, ������, ���� ����� ����Ѵ�.
//���� ��������� ó���Ѵ�.                              Modify by GSB(2003.2.1)
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

  dist := find_dist( MovePos.X, MovePos.Y, X, Y );  //�� ��ǥ ������ �Ÿ��� Pixel�� return �Ѵ�.

  if dist < 3 then
  begin
    //if DrawingSlideBuffer then
    //DrawVirtualDot();
    exit;                                 //���̳� ���콺�� õõ�� ������ ��� Anti-Aliasing�� �˰��� ���������� �״´�.
  end;                                    //�׸��� ���콺�� �������� ���� ����Ƽ ���ϸ� ���� ��Ȱ�� �Ѵ�. �̰��� �� ũ���ϸ�
                                          //���� ���������� ���콺�� �����Ӱ��� ������ ���ϸ� �����´�.
                                          //���, ������ ��ǥ�� ���� ��ǥ���� �Ÿ��� 3Pixel �����̸� �����ϰ� ����������.

  //if fPressLineSize > fDrawPenWidth then fPressLineSize:=fDrawPenWidth; //MessageDlg('Exiting ect Pascal application.', mtInformation,   [mbOk], 0);


  iMPointNum := iMPointNum+1;

  MovePos := Point(X, Y);

  //////////////////////////////////////////////////////////////////////////////

  DrawAllPen( Rec, False, DrawingSlideBuffer );

  //////////////////////////////////////////////////////////////////////////////

  iMPointNum := 0;

  ptMPoint[iMPointNum] := ptMPoint[1];   //�Ʒ��� Point �迭(ptMPoint)�� BSpline ��� �׸��� ���� ���̴�.
  iMPointNum := iMPointNum+1;            //Spline ��� 4���� ��ǥ�� �䱸�Ѵ�.
                                         //4���� ��ǥ�� �ָ�(0~3) 1~2 ������ ��ǥ�� ���� ��� �׸���.
  ptMPoint[iMPointNum] := ptMPoint[2];   // 0~1, 2~3�� ��ǥ�� 1~2������ ��� ����, ���� � ������ ��ģ��.
  iMPointNum := iMPointNum+1;

  ptMPoint[iMPointNum] := ptMPoint[3];
  iMPointNum := iMPointNum+1;

  DownFirst := False;
end;

////////////////////////////////////////////////////////////////////////////////
//����, ������, ���� ����� ����Ѵ�.
//���� ���� ó���Ѵ�.                                    Modify by GSB(2003.2.1)
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
//����, ������, ���� ����� ����Ѵ�.
//���� ������ ó���Ѵ�.                                  Modify by GSB(2003.2.1)
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
//����, ������, ���� ����� ����Ѵ�.
//���� ��������� ó���Ѵ�.                              Modify by GSB(2003.2.1)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.MouseMove_LightPen(Rec: TEventStruct_Ver20; X, Y: Integer; DrawingSlideBuffer: Boolean);
 var
  dist: Integer;
begin

  if (iLast_x=X) and (iLast_y=Y) then exit;

  iLast_x:=X;
  iLast_y:=Y;

  dist := find_dist( MovePos.X, MovePos.Y, X, Y );  //�� ��ǥ ������ �Ÿ��� Pixel�� return �Ѵ�.

  if dist < 3 then exit;

  MovePos := Point(X, Y);

  ptMPoint[iMPointNum].X := X;
  ptMPoint[iMPointNum].Y := Y;
  iMPointNum := iMPointNum+1;

  //////////////////////////////////////////////////////////////////////////////

  DrawLightPen( Rec, DrawingSlideBuffer );

  //////////////////////////////////////////////////////////////////////////////

  iMPointNum := 0;

  ptMPoint[iMPointNum] := ptMPoint[1];   //�Ʒ��� Point �迭(ptMPoint)�� BSpline ��� �׸��� ���� ���̴�.
  iMPointNum := iMPointNum+1;            //Spline ��� 4���� ��ǥ�� �䱸�Ѵ�.
                                         //4���� ��ǥ�� �ָ�(0~3) 1~2 ������ ��ǥ�� ���� ��� �׸���.
  ptMPoint[iMPointNum] := ptMPoint[2];   // 0~1, 2~3�� ��ǥ�� 1~2������ ��� ����, ���� � ������ ��ģ��.
  iMPointNum := iMPointNum+1;

  ptMPoint[iMPointNum] := ptMPoint[3];
  iMPointNum := iMPointNum+1;

  DownFirst := False;
end;

////////////////////////////////////////////////////////////////////////////////
//����, ������, ���� ����� ����Ѵ�.
//���� ���� ó���Ѵ�.                                    Modify by GSB(2003.2.1)
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

//Integer������ ����Ϸ��� Return ���� �޾� trunc �ϱ� �ٶ���.
//BSPline���� ����ϴ� ������ ���� ������ ��� x ��ǥ�� Return �Ѵ�.
function TfrmIPro.find_x( t:double; x10,x11,x12,x13:Integer ): Single;
begin
  Result := (((-x10+3*x11-3*x12+x13)*t*t*t+(3*x10-6*x11+3*x12)*t*t+(-3*x10+3*x12)*t+x10+4*x11+x12)/6); //+0.2; //000000000000008);
end;

//Integer������ ����Ϸ��� Return ���� �޾� trunc �ϱ� �ٶ���.
//BSPline���� ����ϴ� ������ ���� ������ ��� y ��ǥ�� Return �Ѵ�.
function TfrmIPro.find_y( t:double; y10,y11,y12,y13:Integer ): Single;
begin
  Result := (((-y10+3*y11-3*y12+y13)*t*t*t+(3*y10-6*y11+3*y12)*t*t+(-3*y10+3*y12)*t+y10+4*y11+y12)/6);  //+0.2; //000000000000008);
end;

//2���� ��ǥ �Ÿ��� Pixel�� ��ȯ�Ѵ�.
function TfrmIPro.find_dist( x1, y1, x2, y2: Integer ): Integer;
begin
  Result := trunc( Sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) ) );
end;

procedure TfrmIPro.DrawPenBackDot( fLine: Single );
var
  Theta: Single;                   //Extended;  ��ǻ���� ���μ��� ���� ó�� �ɷ��� ���̰� ����.
  rDx1, rDy1: Single;              //Extended;  32Bit���μ����� 32Bit ������ ���� ������ �����Ѵ�.
begin                              //64Bit�� �翬�� 64Bit ������ ����ϴ� ���� ����.
                                   //��, �������� �����ؾ� ����ȭ �� �� �ִ�.
                                   //����� 32Bit�� �������� �ߴ�. Modify by GSB(2003.2.1)
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
  // Update�� ������ �����Ѵ�.
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
        //if brusnEnd then DrawBrushPenEnd()    //�� Bspline ��� �׸���.
        //else DrawBrushPen();                  //�� Bspline ��� ������ �׸���.

        if brusnEnd then DrawBrushPenEnd()      //�� Bspline ��� �׸���.
        else DrawBrushPen();                    //�� Bspline ��� ������ �׸���.
      end;
      esSignPen_Ver20  : DrawSignPen();       //������
      esNormalPen_Ver20: DrawPencil();        //����
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
   /////////////////////// ImgSlide�� ���� ���� �׸��� /////////////////////////

   Color := SetAlpha( fDrawPenColor,95-trunc((fPressLineSize-1)*15) ); //������ �󵵸� �ٸ� �ͺ��� �帮�� �Ͽ���.

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
  Theta: Single;                   //Extended;  ��ǻ���� ���μ��� ���� ó�� �ɷ��� ���̰� ����.
  TmpPoly: TPolygon32;
  rDx1, rDy1: Single;              //Extended;  32Bit���μ����� 32Bit ������ ���� ������ �����Ѵ�.
begin                              //64Bit�� �翬�� 64Bit ������ ����ϴ� ���� ����.
                                   //��, �������� �����ؾ� ����ȭ �� �� �ִ�.
                                   //����� 32Bit�� �������� �ߴ�. Modify by GSB(2003.2.1)
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
//�ױ�� - �� �з¿� ���� ���Ⱑ ���Ѵ�(������ 1~7�ܰ�)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.DrawBrushPen();
 var
    t, inc, x, y: Single;    //, new_x, new_y
    dist: Integer;
    color: TColor32;
begin
   //Spline���� �׸� ���� �׸���.  //pbSlide[gCurrLoadedFileIndex].DrawMode := dmBlend;    //dmOpaque;
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

   /////////////////////// ImgSlide�� ���� ���� �׸���.

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
//�ױ���� �ϳ��� ������ ȹ�� ���κ��� �����ϰ� �ϴ� ��ƾ
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

   if dist<1 then exit;                                                         //Ȥ upgrade�ϸ鼭 �Ǽ��� �ٿ��� ����Ű�� ���� ����Ͽ� ...
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

     /////////////   ���� �ǽð� ���������� �׸��� ���� While �� �ȿ� �д�. ���� ���ϰ� ���� �ɸ���.

     //if penwidth<0 then penwidth:=0;
     if penwidth<0.01 then penwidth:=0.01;  //0.2

     /////////////////////////////////// DrawDott���� �׸� ���� ��´�.

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
//������ - �� �з°� �ӵ��� ���� ���� �󵵰� ���Ѵ�(������ 1~7�ܰ�)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.DrawSignPen();
 var
    t, inc, x, y : Single;
    dist: Integer;
    color: TColor32;
begin                                                                                //Spline���� �׸� ���� �׸���.  //pbSlide[gCurrLoadedFileIndex].DrawMode := dmBlend;    //dmOpaque;
   x     := find_x( 0, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   y     := find_y( 0, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );
   new_x := find_x( 0.99, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   new_y := find_y( 0.99, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

   dist := find_dist( trunc(x), trunc(y), trunc(new_x), trunc(new_y) );

   if dist<1 then exit;                          //Ȥ upgrade�ϸ鼭 �Ǽ��� �ٿ��� ����Ű�� ���� ����Ͽ� ...
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

   /////////////////////// ImgSlide�� ���� ���� �׸��� -> MouseDown_Pen���� �׸���.

   while t < 1 do
   begin
     new_x := find_x( t, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
     new_y := find_y( t, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

     ///////////////////////////////////  DrawPenDot���� �׸� ���� ImgSlide�� ��´�.

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
//���� - ������ ����� �׸���(������ 1~7�ܰ�)
////////////////////////////////////////////////////////////////////////////////
procedure TfrmIPro.DrawPencil();
 var
    t, inc, x, y: Single;
    dist: Integer;
    color: TColor32;
begin
                                                                                //Spline���� �׸� ���� �׸���.  //pbSlide[gCurrLoadedFileIndex].DrawMode := dmBlend;    //dmOpaque;
   x     := find_x( 0, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   y     := find_y( 0, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );
   new_x := find_x( 0.99, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
   new_y := find_y( 0.99, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

   dist := find_dist( trunc(x), trunc(y), trunc(new_x), trunc(new_y) );

   if dist<1 then exit;                                                        //Ȥ upgrade�ϸ鼭 �Ǽ��� �ٿ��� ����Ű�� ���� ����Ͽ� ...
   if dist>100 then dist:=100;

   //�� �κе� MouseDown_Pen �θ� �������� �׷��� �ϸ� ���� ����... ���� ���� �̰��� �д�.

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

   /////////////////////// ImgSlide�� ���� ���� �׸��� -> MouseDown_Pen���� �׸���.

   while t < 1 do
   begin
     new_x := find_x( t, ptMPoint[0].X, ptMPoint[1].X, ptMPoint[2].X, ptMPoint[3].X );
     new_y := find_y( t, ptMPoint[0].Y, ptMPoint[1].Y, ptMPoint[2].Y, ptMPoint[3].Y );

     ///////////////////////////////////  DrawPenDot���� �׸� ���� ImgSlide�� ��´�.

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
   inc:=0.01;  //263;  //��� �׸� ��� Pixel�� ��� �Ǵ� 1���� � ���� ������ 1/0.01 ���� ���� �ȴ�(100��)
               //�� ���� 0.���� 10���� ��� �ȴ�. ���� ������ �ʴ�. �밳 0.1�� ����Ѵ�.

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

  if (iLast_x=x) and (iLast_y=y) then exit;       //�̰��� �ʿ��� ������ �������� ���� �ڸ��� 2�� ���� ���� �����ϴ� ���̴�.
                                                  //ª�� �Ÿ��� 100���� ���� ��� ������ �ӵ��� ����Ƽ�� ���� �߿��ϴ�.
  iLast_x := x;
  iLast_y := y;

  DrawLightPenDot( Rec, ACanvas, ret45.X,   ret45.Y,   ret45.per1 );
  DrawLightPenDot( Rec, ACanvas, ret45.X+1, ret45.Y,   ret45.per2 );
  DrawLightPenDot( Rec, ACanvas, ret45.X+1, ret45.Y+1, ret45.per3 );
  DrawLightPenDot( Rec, ACanvas, ret45.X,   ret45.Y+1, ret45.per4 );

end;

//����(d)�� ����ؼ� x,y ��ǥ�� ���̳� ������ ��� �Լ�
procedure TfrmIPro.DrawLightPenDot( Rec: TEventStruct_Ver20; ACanvas: TBitmap32; x, y, d: Integer );
 var
  color: TColor32;
begin
                                            //fDrawPenColor  gColor;
  color := SetAlpha( Color32(Rec.EventColor), 1 );  //Color32(clYellow), d div 12  );

  x:=x-7;
  y:=y-(Rec.PenWidth div 2);

  ACanvas.FillRectTS( x, y, x+14, y+Rec.PenWidth+1, color ); //�簢
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

    if ((DownPos.X=MovePos.X) and (DownPos.Y=MovePos.Y)) or ( (bUpDown=False) and (MovePos.Y-Y > 0) ) then       //�Ʒ����� ���� ����  DownUp,
      begin
        bUpDown := True;
        if distX < 120 then
          begin
            iDownUp := iDownUp + 1;
            iUpDownLastX := X;
          end;
      end
    else if ((DownPos.X=MovePos.X) and (DownPos.Y=MovePos.Y)) or ( (bUpDown=True) and (MovePos.Y-Y < 0) ) then   //������ �Ʒ��� ����  UpDown,
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

  if DrawingSlideBuffer then                                                    // Undo / Redo �۾���
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

  while i < X2 do   //300�� Pixel ����
    begin           //X     Y         //2*PI    ���ļ�                      //��� ����
      SetPenDot( ACanvas, i+0.0, Y1-sin( (2*PI)/(22)*(118-fDrawPenWidth/5+(i-iImsi) ) )*(1.5+(fDrawPenWidth/15)), fDrawPenColor );
      i:=i+1;
    end;
end;

